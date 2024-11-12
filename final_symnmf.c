#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int i;
int j;
int k;

int ERROR(){
    printf("An Error Has Ocurred");
    exit(1);
}



/**
 * @brief Initializes cluster list
 * 
 * This function takes the data matrix, pointer to cluster list, an int K and number of dimensions D.
 * It creates a linked list of size D of clusters and initiates their centroids.
 * 
 * @param DATA a pointer to a 2d float matrix of data
 * @param cluster_list Pointer to an initiated linked list of clusters
 * @param K Number of clusters
 * @param D Number of dimensions
 * @return None, all changed parameters are called by reference.
 */

typedef struct{
    double **entries;
    double **transposed;
    int rows;
    int cols;
} Matrix;
Matrix H;

Matrix *SYM(Matrix *A){
    Matrix *B;
    B -> entries = newMatrix(A ->rows, A -> rows);
    for(i=0; i< A -> rows; i++){
        for(j=0; j< A -> cols; j++){
            B -> entries [i][j] = distance(A,i,j);
        }
    }
    B -> rows, B -> cols = A -> cols, A -> cols; 
    return B;
}

Matrix *DDG(Matrix *A){
    double num;
    Matrix *B;
    B ->entries = newMatrix(1,A -> rows);
    for (i = 0; i < A ->rows ; i++){
        for(j = 0; j < A -> rows; j++){
            num += A -> entries[i][j];
        }
        B -> entries[i] = &num;
        num = 0;
    }
    B -> rows = 1;
    B -> cols = A -> rows;
    if (!TransposedMatrix(&B)){
        ERROR();
    }
    return B;
}
Matrix *newMatrix(int n, int m){
    Matrix *M;
    M -> entries =(double**)malloc(n * sizeof(double*));
    if (M-> entries == NULL){
        ERROR();
    }
    for (i = 0; i < n; i++){
        M -> entries[i] = (double*)malloc(m * sizeof(double));
        if (M -> entries[i] == NULL){
            ERROR();
        }
    }
    M -> rows = n;
    M -> cols = m;
    return M;
}


double Distance(Matrix *A, int i, int j){
    if (A -> rows != A -> cols){
        ERROR();
    }
    double num;
    for (k = 0; k < A ->rows; k++){
        num = pow(A -> entries[i][k]- A -> entries[k][j], 2)/2;
    }
    return num;
}
/**
 * Computes the matrix product of two matrices A and B and stores the result in C.
 * @param A: A 2D array representing the matrix A of size n x k.
 * @param B: A 2D array representing the matrix B of size k x m.
 * @raises NULL: If memory allocation for the result matrix fails.
 */
Matrix *MatrixMult(double **A, double **B){
    double num;
    Matrix *C;
    if (sizeof(A[0]) != sizeof(B)){
        ERROR();
    }
    C -> entries = newMatrix(sizeof(A), sizeof(B[0]));
    if( C -> entries == NULL){
        ERROR();
    }
    C -> rows, C -> cols = sizeof(A), sizeof(B[0]);
    for(i = 0; i < sizeof(A); i++){
        for(j = 0; j < sizeof(B[0]); j++){
            for(k = 0; k < sizeof(B); k++){
                num += A[i][k] * B[k][j];
            }
        C -> entries[i][j] = num;
        num =0;
        }
    }
    return C
}
Matrix *normalizedSYM(Matrix *A){
    Matrix *D;
    D -> entries = newMatrix(1, A -> rows);
    D -> entries = DDG(A);
    for(i =0; i < A ->rows; i++){
        D ->entries[0][i] = 1 / sqrt(D -> entries[0][i]);
    }
    TransposedMatrix(D);
    Matrix *B;
    B -> entries = MatrixMult(MatrixMult(D -> entries, A -> entries), D -> transposed);
    freeMatrix(&D);
    B -> cols, B -> rows = sizeof(A), sizeof(A);
    return A;
}

TransposedMatrix(Matrix *M){
    M -> transposed = newMatrix(M ->cols, M -> rows);
    for (j = 0; j < M ->cols; j++ ){
        for(i = 0; i < M -> rows; i++){
            M -> transposed[j][i] = M -> entries[i][j];
        }
    }
}
double MatrixDistance(Matrix *A, double **B){
    double sum = 0;
    if (A -> rows != sizeof(B) || A -> cols != sizeof(B[0])) {
        ERROR();
        return NULL;
    }
    for (i = 0; i < A -> rows; i++){
        for (j = 0; j < A -> cols; j++){  
            sum += pow((A -> entries[i][j] - B[i][j]),2);
        }
    }
    return sum;
}




int isVALID(char *GOAL){
    if (strcmp(GOAL, "sym") == 0) {
}
 if (strcmp(GOAL, "ddg") == 0) {}
  if (strcmp(GOAL, "norm") == 0) {}
   if (strcmp(GOAL, "sym") == 0) {}


int main(int argc, char *argv[]) {
    char *filePath = "vectors.txt";
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }


    if (argc != 2){
        ERROR();
    }
    char *filePath = argv[1];\\ צריךך לבדןק את האורך
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        ERROR()
    }
    while (fscanf(file, "%d", &DATA[index]) == 1) {
        index++;
    }
}
