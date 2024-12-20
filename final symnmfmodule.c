#include "Python.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "symnmf.h"
#define EPSILON 0.0001
#define MAX_ITER 300
#define BETA 0.5

char *py_string_to_c(PyObject *py_string) {
    if (!PyUnicode_Check(py_string)) {
        PyErr_SetString(PyExc_TypeError, "Input must be a string");
        return NULL;
    }
    const char *str = PyUnicode_AsUTF8(py_string);
    if (str == NULL) {
        PyErr_SetString(PyExc_ValueError, "Failed to convert Python string to UTF-8");
        return NULL;
    }
    char *c_string = strdup(str);
    if (c_string == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for C string");
        return NULL;
    }
    return c_string;
}
/**
 * @brief Converts a C matrix (2D array) to a Python list of lists.
 *
 * @param matrix A pointer to the C matrix (2D array) to convert.
 * @param rows The number of rows in the matrix.
 * @param cols The number of columns in the matrix.
 * @return A Python object representing the matrix as a list of lists.
 */
PyObject *Matrixtopyobject(Matrix *matrix) {
    int rows = matrix->rows;
    int cols = matrix->cols;
    PyObject *py_matrix = PyList_New(rows);  // Create the outer Python list
    if (!py_matrix) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for the Python matrix.");
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        PyObject *py_row = PyList_New(cols);  // Create a Python list for each row
        if (!py_row) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for a Python row.");
            Py_DECREF(py_matrix);  // Free the partially created matrix
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            PyObject *py_value = PyFloat_FromDouble(matrix->entries[i][j]);  // Convert C double to Python float
            if (!py_value) {
                PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for a Python value.");
                Py_DECREF(py_row);  
                Py_DECREF(py_matrix);  
                return NULL;
            }
            PyList_SET_ITEM(py_row, j, py_value);  // Add the value to the Python row
        }

        PyList_SET_ITEM(py_matrix, i, py_row);  // Add the row to the Python matrix
    }

    return py_matrix;
}
void copy_data(double **DATA, double **target, int N,int M){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < M; j++){
            target[i][j] = DATA[i][j];
        }
    }

}

int ColumnSize(PyObject *DATA_py) {
    PyObject *rowObj;

    rowObj = PyList_GetItem(DATA_py, 0);
    if (!rowObj) {
        return 0; 
    }
    if (!PyList_Check(rowObj)) {
        return 0;  // Error: Not a list of lists
    }
    return (int)PyList_Size(rowObj);
}
int Data_py_to_c(PyObject *DATA_py, Matrix *M) {
    int i, j;
    PyObject *row,*value;
    for (i = 0; i < M->rows; i++) {
        row = PyList_GetItem(DATA_py, i);
        if (!PyList_Check(row)) {
            PyErr_SetString(PyExc_TypeError, "Each row in the matrix must be a list");
            free_matrix(M);
            return 0;
        }
        for (j = 0; j < M->cols; j++) {
            value = PyList_GetItem(row, j);
            if (!PyFloat_Check(value)) {
                PyErr_SetString(PyExc_TypeError, "Matrix elements must be floats");
                free_matrix(M);
                return 0;
            }
            M->entries[i][j] = PyFloat_AsDouble(value);  // Corrected to access the matrix correctly
        }
    }
    return 1;
}

Matrix *convertMPyToMatrix(PyObject *DATA_py){
    int n;
    int m;
    Matrix *M;
    n = (int)PyList_Size(DATA_py);
    if (n == 0) {
        PyErr_SetString(PyExc_ValueError, "The input list is empty");
        return NULL;
    }
    m = ColumnSize(DATA_py);
    if (m == 0) {
        PyErr_SetString(PyExc_ValueError, "The input list is empty");
        return NULL;
    }
    M = newMatrix(n, m);
    if (M == NULL){
        free_matrix(M);
        return NULL;
    }
    Data_py_to_c(DATA_py, M);
    if(M -> entries == NULL){
        free_matrix(M);
    }
    return M;
}
void printt(double **matrix, int n,int m){
    int i, j;
    for(i = 0; i < n; i++){
        for (j = 0; j < m - 1; j++){
            printf("%.4f,", matrix[i][j]);
        }
        printf("%.4f\n", matrix[i][m - 1]);
    }
}


Matrix *updateH (Matrix *H, Matrix *W) {
    int i,j;
    double **tmp = alocateMemoForEntries(H->rows, H->cols);
    double **tmp_t = NULL;
    Matrix *returned = newMatrix(H->rows, H->cols); 
    double **A = NULL, **B = NULL, **C = NULL; 
    print_matrix(H);
    printf("\n");
    A = MatrixMult(H->entries, H->transposed, W->cols, W->cols, H->cols);
    printt(A, H->rows, H->rows);
    B = MatrixMult(A, H->entries, W->cols ,H->cols, W->cols);
    C = MatrixMult(W->entries, H->entries, W->cols, H->cols, W->cols);
    if (A == NULL || B == NULL || C == NULL){
        free_data(A, W->cols);
        free_data(B, W->cols);
        free_data(C, W->cols);
        return NULL;
       }
    for(i = 0; i < H->rows; i++){
        for(j = 0; j < H->cols; j++){
            tmp[i][j] = fabs(1-0.5 + 0.5* (C[i][j]/ B[i][j]));
        }
    }
    free_data(A, W->cols);
    free_data(B, W->cols);
    free_data(C, W->cols);
    returned->entries = tmp;
    tmp_t = TransposedMatrix(returned);
    if (!tmp_t ){
        return NULL;
    }
    returned->transposed = tmp_t;
    return returned;
}

Matrix *SYMNMFfunc(Matrix *H, Matrix *W){
    int iter = 0;
    Matrix *updated = (Matrix *)malloc(sizeof(Matrix));
    Matrix *prev = (Matrix *)malloc(sizeof(Matrix)); 
    double err;
    prev = H;
    while (iter < MAX_ITER){
        
        updated = updateH(prev,W);

        if(!updated){
            return NULL;
        }
        err = MatrixDistance(updated, prev->entries);
        if ( err == -1){
            free_matrix(prev);
            free_matrix(updated);
            return NULL;
        }
        
        if (err < EPSILON){
            free_matrix(prev);
            return updated;
        }  
        iter++;
        free_matrix(prev);
        prev = (Matrix *)malloc (sizeof(Matrix));
        prev = updated;
    } 
    free_matrix(prev);
    return updated;
}

PyObject* SYMNMF(PyObject *self, PyObject *args){
    PyObject *H_py, *W_py, *returned;
    Matrix *H = (Matrix *)malloc(sizeof(Matrix));
    Matrix *W =(Matrix *)malloc(sizeof(Matrix));
    Matrix *returned_c = (Matrix *)malloc(sizeof(Matrix));
    double **tmp1 = NULL;
    if (!PyArg_ParseTuple(args, "OO", &W_py, &H_py)) {
        return NULL;
    }
    H =convertMPyToMatrix(H_py);
    W = convertMPyToMatrix(W_py);
    if(H == NULL || W == NULL){
        PyErr_SetString(PyExc_MemoryError, "Failed to convert input matrices");
        free_matrix(H); 
        free_matrix(W);
        return NULL;
    } 
    tmp1= TransposedMatrix(H);
    if(!tmp1){
        free_matrix(H);
        free_matrix(W);
        PyErr_SetString(PyExc_MemoryError, "Failed to convert input matrices");
        return NULL;
    }
    H->transposed = tmp1;
    returned_c = SYMNMFfunc(H, W);
    if (returned_c == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to convert matrix to Python object");
        free_matrix(W); 
        return NULL;
    }
    free_matrix(W);
    returned = Matrixtopyobject(returned_c);
    if (returned == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to convert matrix to Python object");
        free_matrix(returned_c);
        return NULL;
    }
   
    Py_INCREF(returned);
    free_matrix(returned_c);
    return returned;
}




PyObject* processGOAL(PyObject *self, PyObject *args) {
    const char *GOAL;
    const char *FILEPATH;
    PyObject *result_py;
    Matrix *result = (Matrix *)malloc(sizeof(Matrix)); 
    if (!PyArg_ParseTuple(args, "ss", &GOAL, &FILEPATH)) {
        return NULL;
    }
    Matrix *DATA = (Matrix *)malloc(sizeof(Matrix));
    DATA = parsefiletoMatrix(FILEPATH);
    Matrix *A = (Matrix *)malloc(sizeof(Matrix)); 
    A = SYM (DATA);
    
     if (strcmp(GOAL, "sym") == 0) {
        result = A;
    } 
    else if (strcmp(GOAL, "ddg") == 0) {
        result = DDG(A);
        free_matrix(A);
    } 
    else if (strcmp(GOAL, "norm") == 0) {
        result = normalizedSYM(A);
        free_matrix(A);
    } 
    else {
        PyErr_SetString(PyExc_ValueError, "Invalid goal");
        free_matrix(result);
        return NULL;}
    result_py = Matrixtopyobject(result);
    free_matrix(result);
    return result_py;
}
static PyMethodDef symnmfMethods[] = {
    {"processGOAL",  
      (PyCFunction) processGOAL,  
      METH_VARARGS,           
      PyDoc_STR("Perform the goal that is given by the user")},
    {"SYMNMF",  
    (PyCFunction) SYMNMF,  
      METH_VARARGS,           
      PyDoc_STR("Perform the SYMNMF goal that is given by the user")},
    {NULL, NULL, 0, NULL} 
};



static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf", 
    NULL, 
    -1,
    symnmfMethods 
};



PyMODINIT_FUNC PyInit_symnmf(void)
{
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) {
        return NULL;
    }
    PyModule_AddFunctions(m, symnmfMethods);
    return m;
}
