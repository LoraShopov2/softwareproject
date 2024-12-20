
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h> 
#include "symnmf.h"

int ERROR(){
printf("An Error Has Ocurred");
exit(1);
}
void check_allocation(void *ptr) {
    if (ptr == NULL) {
        perror("Memory allocation failed2");
        exit(1);
    }
}
void free_data(double **DATA, int rows) {
    int i;
    if (DATA) {
        for ( i = 0; i < rows; i++) {
            free(DATA[i]);
        }
        free(DATA);
    }
}
Matrix *newMatrix(int n, int m) {
    Matrix *M = (Matrix *)malloc(sizeof(Matrix));
    if (!M) {
        perror("Memory allocation failed for Matrix structure");
        exit(1);
    }

    M->entries = alocateMemoForEntries(n, m);
    if (!M->entries) {
        free(M);
        perror("Memory allocation failed for matrix entries");
        exit(1);
    }

    M->rows = n;
    M->cols = m;
    M->transposed = NULL;
    return M;
}
void print_matrix(Matrix *matrix) {
    int i;
    int j;
    if (matrix == NULL || matrix->entries == NULL) {
        return;
    }
    for(i = 0; i < matrix->rows; i++){
        for (j = 0; j < matrix->cols - 1; j++){
            printf("%.4f,", matrix->entries[i][j]);
        }
        printf("%.4f\n", matrix->entries[i][matrix->cols - 1]);
    }
}
void free_matrix(Matrix *matrix) {
    int i, j;
    if (matrix == NULL) return; 
    
    if (matrix->entries) {
        for (i = 0; i < matrix->rows; i++) {
            if (matrix->entries[i]) {
                free(matrix->entries[i]);
            }
        }
        free(matrix->entries);
        matrix->entries = NULL; 
    }
    if (matrix->transposed) {
        for (j = 0; j < matrix->cols; j++) {
            if (matrix->transposed[j]) {
                free(matrix->transposed[j]);
            }
        }
        free(matrix->transposed);
        matrix->transposed = NULL; 
    }

    free(matrix); 
    matrix = NULL; 
}



Matrix *SYM(Matrix *DATA) {
    int i, j;
    Matrix *A = newMatrix(DATA->rows, DATA->rows);
    for (i = 0; i < DATA->rows; i++) {
        for (j = 0; j < DATA->rows; j++) {
            if (i != j) {
                double distance = Distance(DATA->entries[i], DATA->entries[j], DATA->cols);
                A->entries[i][j] = exp(-distance / 2.0);
            } else {
                A->entries[i][j] = 0.0;
            }
        }
    }
    return A;
}
double colomnSUM(Matrix *A,int i){
    double sum = 0.0;
    int j;
    for (j = 0; j < A->cols; j++){
        sum += A ->entries[i][j];
    }
    return sum;
}
Matrix *DDG(Matrix *A){
    Matrix *ddg = NULL;
    double sum;
    int i;
    int j;
    ddg = newMatrix(A->rows, A->rows);
    if (ddg == NULL) return NULL;
    for (i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            if( i==j){
                sum = colomnSUM(A, i);
                ddg->entries[i][i] = sum;
            }
            else{
                ddg ->entries[i][j] = 0.0;
            }
        }
        
    }
    ddg->transposed = TransposedMatrix(ddg);
    return ddg;
}


Matrix *extractData(const char* file_name) {

    int n=0, dim = 1, i = 0, j = 0;
    char line[1000];
    char *eptr, *token;
    FILE* file = fopen(file_name, "r");
    Matrix *DATA;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\0') { n++; }
        if (n == 1) {
            while (line[i] != '\0') {
                if (line[i] == ',') { dim++; }
                i++;
            }
            i = 0;
        }
    }
    fseek(file, 0, SEEK_SET);
    DATA = newMatrix(n,dim);
    if (DATA == NULL) { fclose(file); return NULL; } 

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '\0') { break; }
        token = strtok(line, ",");
        j = 0;
        while (token != 0) {
            DATA->entries[i][j] = strtod(token, &eptr);
            token = strtok(0, ",");
            j++;
        }
        i++;
    }
    fclose(file);
    return DATA;
}

/**
 * @brief Read a file line by line and call process_line to populate the list of entroies.
 *
 * @param file_path The path to the input file.
 * @return a pinter to a matrix of the vectors that are given in the file.
 */
Matrix *parsefiletoMatrix(const char *file_path) {
    Matrix *A = NULL;
    A = extractData(file_path);
    if (!A) {
        perror("Failed to allocate Matrix");
        return NULL;
    }
    return A;          
}



double Distance(double *a, double *b, int length) {
    double sum = 0.0;
    int i;
    for (i = 0; i < length; i++) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sum;
}
/**
 * Computes the matrix product of two matrices A and B and stores the result in C.
 * @param A: A 2D array representing the matrix A of size n x k.
 * @param B: A 2D array representing the matrix B of size k x m.
 * @raises NULL: If memory allocation for the result matrix fails.
 */
double **MatrixMult(double **A, double **B,int N, int M,int K){
    double num = 0.0;
    int i;
    int j;
    int l;
    
    double **entries = alocateMemoForEntries(N,M);
    if (!entries){
        ERROR();
        return NULL;
    }
    for(i = 0; i < N; i++){
        for(j = 0; j < M; j++){
            for(l = 0; l < K; l++){
                num += A[i][l] * B[l][j];
            }
            entries[i][j] = num;
            num =0.0;
        }
    }
    return entries;
}
Matrix *normalizedSYM(Matrix *A){
    int i;
    int j;
    Matrix *W = NULL;
    Matrix *D = NULL;
    D = DDG(A);
    W = newMatrix(A->cols, A->cols);
    for(i =0; i < A ->rows; i++){
        for(j = 0; j < A ->cols; j++){
                W->entries[i][j] = A->entries[i][j] / (sqrt(D->entries[i][i]) * sqrt(D->entries[j][j]));
        }
    }
    free_matrix(D);
    W -> cols = A->cols;
    W -> rows = A->cols;
    return W;
}

double **alocateMemoForEntries(int n, int m) {
    int i, j;
    double** Matrix;

    Matrix = (double**)malloc(n * sizeof(double*));
    if (Matrix == NULL) {
        fprintf(stderr, "Memory allocation failed!3\n");
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        Matrix[i] = (double*)calloc(m, sizeof(double));
        if (Matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!1\n");
            for (j = 0; j < i; j++) {
                free(Matrix[j]);
            }
            free(Matrix);
            return NULL;
        }
    }
    return Matrix;
}

double **TransposedMatrix(Matrix *M) {
    int i, j;
    double **entries = alocateMemoForEntries(M->cols, M->rows);
    if (!entries) {
            perror("Failed to allocate memory for transposed matrix");
            return 0;
        }
    for (j = 0; j < M ->cols; j++ ){
        for(i = 0; i < M -> rows; i++){
            entries[j][i] = M -> entries[i][j];
        }
    }
    return entries;
}
    
double MatrixDistance(Matrix *A, double **B){
    double sum = 0;
    int i= 0;
    int j= 0;
    if (!A || !B){
        return -1;
    }
    for (i = 0; i < A -> rows; i++){
        for (j = 0; j < A -> cols; j++){  
            sum += pow((A -> entries[i][j] - B[i][j]),2);
        }
    }
    return sum;
}
int main(int argc, char **argv) {
    char *GOAL = argv[1];
    char *FILEPATH = argv[2];
    Matrix *A = NULL;
    Matrix *result = NULL;
    Matrix *DATA = NULL;
    if (argc < 3) {
        printf("Insufficient arguments provided\n");
        return 1;
    }
    DATA = parsefiletoMatrix(FILEPATH);
    if (DATA == NULL || DATA->entries == NULL) {
        printf("Error: Data matrix is NULL or uninitialized\n");
        return 1;
    }
    A = SYM(DATA);
    if (strcmp(GOAL, "sym") == 0) {
        print_matrix(A);
        free_matrix(A);
    } else if (strcmp(GOAL, "ddg") == 0) {
        result = DDG(A);
        print_matrix(result);
        free_matrix(A);
    } else if (strcmp(GOAL, "norm") == 0) {
    
        result = normalizedSYM(A);
        print_matrix(result);
        free_matrix(A);
    } else {
        printf("Invalid goal specified\n");
        free_matrix(DATA);
        free_matrix(A);
        return 1;}
    free_matrix(DATA);
    free_matrix(result);
    return 0;
}

