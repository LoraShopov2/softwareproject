#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <symnmf.c>

#define EPSILON 0.0001
#define MAX_ITERATIONS 300
#define BETA 0.5

Matrix *H;
Matrix *A;
Matrix *W;
Matrix *D;
double BETA;
int j;
int i;
int n;
int m;

int ColumnSize(PyObject *DATA) {
    PyObject *rowObj = PyList_GetItem(DATA, 0);
    if (!PyList_Check(rowObj)) {
        PyErr_SetString(PyExc_TypeError, "The input list must contain lists");
        return -1; 
    }
    return (int)PyList_Size(rowObj);
}
Matrix *covetMPyToMatrixEntries(PyObject *DATA_py){
    Matrix *M;
      if (!PyArg_ParseTuple(args, "O", &DATA_py)) {
        return NULL;
    }
    n = (int)PyList_Size(obj);
    if (n == 0) {
        PyErr_SetString(PyExc_ValueError, "The input list is empty");
        return NULL;
    }
    m = ColumnSize(obj, 0);
    if (m == 0) {
        PyErr_SetString(PyExc_ValueError, "The input list is empty");
        return NULL;
    }
    M -> entries = newMatrix(n, m);
    M -> entries = PyObjectToMatrix(&DATA_py, &M);
    transposedMatrix(&M);
    return M;
}


int get_vec(PyObject *list, int i, Matrix *M) {
for(j =0; j < M -> cols; j++){
    PyObject *element = PyList_GetItem(sublist, j);
if (!element || !PyFloat_Check(element)) {
    return 0;
}
M -> entries[i][j] = PyFloat_AsDouble(element);
return 1;
}
}




/**
 * @brief Convert a Python list of lists to a matrix.
 * 
 * This function converts a Python list of lists (where each inner list represents a row of the matrix) 
 * to a `Matrix` structure.
 * 
 * @param obj A Python object representing a list of lists.
 * @return A pointer to the `Matrix` structure, or NULL if an error occurs.
 */
int PyobjectToMatrix(PyObject *DATA, Matrix *M) {
    Pyobject *vec;
    for (i = 0; i < M -> rows ; i++) {
         vec = PyList_GetItem(DATA, i)
         if (!vec || !PyList_Check(vec)) {
            PyErr_SetString(PyExc_TypeError, "Expected a list of lists.");
        }
        if (!get_vec(vec , i, &M)){
                ERROR();
                return 0;
        }
    }
    if (!TransposedMatrix(&M)){
        ERROR();
        return 0;
    }
    return 1;
}



int updateH (Matrix *H, Matrix *W, BETA) {
    n = H -> rows;
    double A[n][n] = MatrixMult(MatrixMult(H.entries , H.transposed),H.entries);
    double C[n][n] = MatrixMult(W, H);
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            H -> entries[i][j] = H -> entries[i][j](1-BETA + BETA* (C[i][j]/ A[i][j]));
        }
    }
    H.tranposed = TransposedMatrix(H);
    free(A);
    free(C);
    return H;
}

Matrix SymNMFfunc(Matrix *H, Matrix *A){
    int iter, error;
    double PREV[H -> rows][H -> rows] = H.entries;
    W -> entries = normalizedSYM(&A);
    while (iter < MaxIter){
        H -> entries = updateH(&H, &W, BETA);
        if (distanceMatrix(&H ,PREV) < EPSILON){
            return H;
        } 
        iter++;
        
    }
    return H;
}

static PyObject* SYMNMF_module(PyObject *args){
    PyObject *H_py;
    PyObject *DATA_py;
    PyObject *retured;
    Matrix *prossesed;
    Matrix *H;
    Matrix *A;
    Matrix *DATA;

    
    if(!PyobjectToMatrix(H_py, &H) || !PyobjectToMatrix(DATA_py, &DATA) ){
        
        freeMatrix(&H);
        freeMatrix(&DATA);
        return NULL;
    }
    A = SYM(&DATA);
    if (A == NULL) {
        freeMatrix(A);
        return NULL;
    }
    prossesed = SymNMFfunc(&H, &A);
    if (prossesed == NULL) {
        freeMatrix(A);
        freeMatrix(H);
        FreeMatrix(prossesed);
        return NULL;
    }
    returned = matrixToPyObject(prossesed);
    freeMatrix(prossesed);
    return returned;
}
static PyObject* SYM_moudule(PyObject *args) {
    PyObject *DATA_py, A_py;
    Matrix *DATA;
    Matrix*A;
    if (!PyArg_ParseTuple(args, "O", &DATA_py)) {
        return NULL;
    }
   if(!PyobjectToMatrix(DATA_py, &DATA)) return NULL;
    
    A = &SYM(DATA);
    A_py = MatrixToPyobject(A);
    return A_py;
}
    
static DDG_module(PyObject *args){
    PyObject *DATA_py, D_py;
    Matrix *D;
    Matrix *DATA;
    Matrix *W;
    if (!PyArg_ParseTuple(args, "O", &DATA)) {
        return NULL;
    }
    PyobjectToMatrix(DATA_, &DATA);
    A = &SYM(&DATA);
    A -> entries = &SYM(DATA);
    D = &DDG(&A);
    W ->entries = newMatrix(A ->rows, A ->cols);
    W ->entries MatixMult(D -> transposed, MatrixMult(A -> entries, D -> entries));
    
    return MatrixToPyobject(W);
}


static PyObject *NORM_module(PyObject *args){
    Pyobject *DATA_py;
    Matrix A, D;
    if (!PyArg_ParseTuple(args, "O", &DATA_py)) {
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments.");
        return NULL;
    }
    n = (int)PyList_Size(obj);
    if (n == 0) {
        PyErr_SetString(PyExc_ValueError, "The input list is empty");
        return NULL;
    }
    m = ColumnSize(obj, 0);
    DATA -> entries = initializeMatrix(n, m);
    DATA->entries = PyobjectToMatrix(DATA_py);
    TransposedMatrix(DATA);
    A -> entries = initializeMatrix(n, n);
    A -> entries= &SYM(DATA);
    D -> entries = initializeMatrix(1,A.rows);
}


// Function to free the allocated 2D array
void freeCArray(double** cArray, int n) {
    for (int i = 0; i < n; i++) {
        free(cArray[i]);
    }
    free(cArray);
}
static PyMethodDef symnmfMethods[] = {
    {"symnmf",  
      (PyCFunction) symnmf,  
      METH_VARARGS,           
      PyDoc_STR("Perform the symnmf algorithm and output the clusters")},
    {"sym",  
      (PyCFunction) sym,  
      METH_VARARGS,           
      PyDoc_STR("Calculate and output the similarity matrix")},
    {"ddg",  
      (PyCFunction) ddg,  
      METH_VARARGS,           
      PyDoc_STR("Calculate and output the Diagonal Degree Matrix")},
    {"norm",  
      (PyCFunction) norm,  
      METH_VARARGS,           
      PyDoc_STR("Calculate and output the normalized similarity matrix")},
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
    
    return m;
}



        
