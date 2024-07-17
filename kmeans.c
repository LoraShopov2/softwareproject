/**
 * @file kmeans.c
 * @brief Calculating a k mean clusters from a given input file
 * 
 * This file contains code that given a number k, iterations and input file returns the centroids of the k mean cluster.
 * 
 * @author 322721705 and 211493176
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.001
#define MAX_PATH 200

/**
 * @brief Represents a linked list of points.
 *
 * This structure holds a dynamically allocated array of points and a pointer to the next element in the list.
 * It is used to manage the points assigned to a cluster in a linked list format.
 */
typedef struct POINT_LIST{
    double *head;
    struct POINT_LIST *next;
    int size;
} POINT_LIST;


/**
 * @brief Represents a cluster with its centroid, previous centroid, and list of points.
 *
 * This structure is used in clustering algorithms to manage clusters,
 * including their current and previous centroids and the list of points assigned to the cluster.
 */
typedef struct CLUSTER{
    double *centroid;
    double *prev;
    POINT_LIST *point_list;
    int size;
} CLUSTER;


/**
 * @brief Represents a linked list of clusters.
 *
 * This structure is used to manage multiple clusters in a linked list format.
 */
typedef struct CLUSTER_LIST{
    CLUSTER *head;
    struct CLUSTER_LIST *next;
    int size;
} CLUSTER_LIST;


/**
 * @brief point to a given point list.
 * 
 * This function takes a point in R^D and adds it to the head of a linked list representing point list
 * 
 * @param point_list Pointer to a point list
 * @param point Pointer to a double array representing the point
 * @return None, all changed parameters are called by value
 */
void addPoint( POINT_LIST **point_list , double *point) {
    POINT_LIST *tmp = (POINT_LIST *)calloc(1, sizeof(POINT_LIST));
    if (tmp == NULL) {
            printf("An error has occurred!\n");
            exit(1);
        }
    tmp->head = point;
    tmp->next = *point_list;
    if (*point_list == NULL) {
        tmp->size = 1;
    } else {
        tmp->size = (*point_list)->size + 1;
    }
    *point_list = tmp;
}   


/**
 * @brief Adds cluster to a cluster list
 * 
 * This function adds a cluster to a linked list representing a cluster list, also check memory allocation errors.
 * 
 * @param cluster_list Pointer to a cluster list
 * @param cluste Pointer to a cluster
 * @return None, all changed parameters are called by value
 */
void addCluster(CLUSTER_LIST **cluster_list , CLUSTER *cluster) {
    CLUSTER_LIST *tmp = (CLUSTER_LIST *)calloc(1, sizeof(CLUSTER_LIST));
    if (tmp == NULL) {
            printf("An error has occurred!\n");
            exit(1);
        }
    tmp->head = cluster;
    tmp->next = *cluster_list;
    *cluster_list = tmp;
}


/**
 * @brief Initiates cluster
 * 
 * This function takes a point in R^D and int D.
 * It allocates memory for cluster, checks memory allocation errors and initializes its centroid and prev using the coordinates of the point.
 * 
 * @param point Pointer to a point
 * @param D Number of dimensions
 * @return Pointer to the initiated cluster
 */
CLUSTER *createCluster(double *point, int D){
    CLUSTER *cluster = calloc(1, sizeof(CLUSTER));
    int i;
    if (cluster == NULL){
        printf("An error has occurred!\n");
        exit(1);
    }
    cluster->centroid = (double *)calloc(D+1, sizeof(double));
     if (cluster->centroid == NULL){
        printf("An error has occurred!\n");
        exit(1);
    }
    cluster->prev = (double *)calloc(D+1, sizeof(double));
     if (cluster->prev == NULL){
        printf("An error has occurred!\n");
        exit(1);
    }
    for (i = 0; i < D; i++) {
        cluster->prev[i] = (double) point[i];
        cluster->centroid[i] = (double) point[i];
        
    }
    cluster->point_list = NULL;
    cluster->size = 0;
    return cluster;
}


/**
 * @brief Clears point list in given cluster.
 * 
 * This function frees all memory allocated in cluster list  by iterating on the linked list and using free()
 * 
 * @param cluster Pointer to a point list
 * @return None, all changed parameters are called by value
 */
void clearCluster(CLUSTER *cluster) {
    POINT_LIST *curr = cluster->point_list;
    POINT_LIST *next;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    cluster->point_list = NULL;
    cluster->size = 0;
}


/**
 * @brief Calculates and updates cenetroid of a cluster
 * 
 * This function takes a cluster, iterates over its point list and calculates the mean of each coordinate of point.
 * Then the function updates values if prev and centroid in cluster.
 * 
 * @param cluster Cluster
 * @param D Dimension
 * @return None, all changed parameters are called by value
 */
void updateCentroid(CLUSTER *cluster, int D){
    int i;
    double sum;
    POINT_LIST *curr;
    for (i = 0; i < D; i++){
        sum = 0.0;
        curr = cluster->point_list;
        while (curr != NULL) {
            sum += curr->head[i];
            curr = curr->next;
        }
        cluster->centroid[i] = sum / cluster->size;
    }
}


/**
 * @brief Parses file input
 * 
 * This function a file from stdin, an initiated array, pointer to N and a pointer to D.
 * It iterates through all array lines and parses the char array stream into a 2d float matrix containing all information from input file.
 * 
 * @param file Input from stdin
 * @param array An initiated 2d array pointer
 * @param N Pointer to number of lines in file
 * @param D Pointer to number of dimensions
 * @return None, all changed parameters are called by reference
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void dimantion(int *D){
    char ch;
    while((ch = getchar()) != '\n'){
        if (ch!= '\n'){
            (*D)++;
        }
    } 
}
  void add_element(char  **word, int *size, int *capacity, char element) {
    if (*size >= *capacity) {
        *capacity*= 2;
        *word = (char *)realloc( *word, (*capacity) * sizeof(char));
    }
    *word++ = &element;
    *size++;
    return;
    }

int charToDouble(double **num){
    char ch;
    int size = 0;
    int capacity =0;
    char *word = (char *)calloc(2, sizeof(char));
    char *start = word;
    if (word == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    while (ch = getchar() != ',' || ch != '\n' || ch != EOF ){
    
        add_element(&word, &size, &capacity, ch);
    }
    add_element(&word, &size, &capacity, '\0');
    **num = atof(start);
    free(start);
    if (ch == EOF){
        return 0;
    }
    return 1;
}
void fileParse(FILE *file, double ***array, int *N, int D) {
    fseek(file, 0, SEEK_SET);
    int size = 0;
    int capacity =0;
    int row = 0;
    int col=0;
    char ch;
    int toKeep=1;
    char *word;
    double *num = (double *)malloc(sizeof(double));
    while(toKeep){
        *array = (double **)realloc(*array, (row + 1) * sizeof(double *));
        (*array)[row] = (double *)calloc(*D ,sizeof(double));
        for (col = 0; col < *D; col++){
            toKeep = charToDouble(&num);
            (*array)[row][col] = *num;
        }
        col = 0;
        row++;
       
    }
    free(word);
    free(num);
    *N = row;
    for (col = 0; col < *N ; col++){
        for (row = 0; row < *D ; row++){
            printf("%f ", (*array)[col][row]);
            printf("%d", 5);
        }
    }

}

/**
 * @brief Initializes cluster list
 * 
 * This function takes the data matrix, pointer to cluster list, an int K and number of dimensions D.
 * It creates a linked list of size D of clusters and initiates their centroids.
 * 
 * @param data 2d float matrix of data
 * @param cluster_list Pointer to an initiated linked list of clusters
 * @param K Number of clusters
 * @param D Number of dimensions
 * @return None, all changed parameters are called by value.
 */
void initializeClusters(double **data, CLUSTER_LIST **cluster_list, int K, int D) {
    int i;
    for (i = 0; i < K; i++) {
        CLUSTER *cluster = createCluster(data[i], D);
        addCluster(cluster_list, cluster);
    }
}


/**
 * @brief Calculates distance between two points
 * 
 * This function takes 2 points in Rd and D and calculates the eucledean distance between the two points
 * 
 * @param point_a Point 1
 * @param point_b Point 2
 * @param D Number of dimensions
 * @return Euclidean distance between the 2 points
 */
double calculateDistance(double *point_a, double *point_b,  int D){
    double sum = 0.0;
    double diff = 0.0;
    int i;
    for (i = 0; i < D; i++){
        diff = point_a[i] - point_b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}


/**
 * @brief Finds the closest cluster to a point
 * 
 * This function takes a point and iterates through cluster list in order to find the closest cluster to the given point.
 * 
 * @param point point in Rd 
 * @param cluster_list List of clusters
 * @param D Number of dimensions
 * @return Pointer to the closest cluster to the point
 */
CLUSTER *findClosestCluster(double *point, CLUSTER_LIST *cluster_list, int D) {
    double minDist = 1.0 / 0.0;
    double dist;
    CLUSTER *closestCluster = NULL;
    CLUSTER_LIST *curr = cluster_list;
    while (curr != NULL) {
        dist = calculateDistance(point, curr->head->centroid, D);
        if (dist < minDist) {
            minDist = dist;
            closestCluster = curr->head;
        }
        curr = curr->next;
    }
    return closestCluster;
}

/**
 * @brief Adds all points to the cluster list
 * 
 * This function takes the data matrix, pointer to cluster list, an int N and number of dimensions D.
 * It iterates through all of the points (lines) in the matrix asnd adds each point to its closest cluster.
 * 
 * @param data 2d float matrix of data
 * @param cluster_list Pointer to an initiated linked list of clusters
 * @param N Number of data points
 * @param D Number of dimensions
 * @return None, all changed parameters are called by value.
 */
void addPointsToClusters(double **data, CLUSTER_LIST *cluster_list, int N, int D) {
    int i;
    for (i = 0; i < N; i++) {
        CLUSTER *cluster = findClosestCluster(data[i], cluster_list, D);
        addPoint(&cluster->point_list, data[i]);
        cluster->size++;
    }
}


/**
 * @brief prints cluster centroids
 * 
 * This function takes the cluster list and the number of dimensions and prints the centorid of each cluster.
 * 
 * @param curr Pointer to an initiated linked list of clusters
 * @param D Number of dimensions
 * @return None
 */
void printClusters(CLUSTER_LIST *curr, int D) {
    int i;
    if (curr != NULL){
        printClusters(curr->next, D);
        for (i = 0; i < D; i++) {
            printf("%.4f", curr->head->centroid[i]);
            if (i < D - 1) printf(",");
        }
        printf("\n");
        return;
    }
    else return;
}


/**
 * @brief Checks whether an arg is an int
 * 
 * This function takes a char array and iterates through it. In each iteration it checks if the char is representing a number or not.
 * 
 * @param arg Char array
 * @return 1 if is int 0 otherwise
 */
int isInteger(const char *arg){
    while (*arg){
        if (*arg < '0'|| *arg > '9'){
            return 0;
        }

        *arg++;
    }
    return 1;
}


/**
 * @brief Runs the main logic of the coode
 * 
 * This function takes K, iter and an input file as input.
 * It creates K clusters using iter, and finally prints their centroids.
 * 
 * @param argc num of arguments given by the user
 * @param argv all user input
 * @return 0 if 
 */
int main(int argc, char *argv[]) {
    int K;
    int N;
    int D = 1;
    int iter;
    CLUSTER_LIST *cluster_list = NULL;
    double **data;
    int flag = 0;
    double diff;
    int i;
    CLUSTER_LIST *curr;
    CLUSTER_LIST *next;
    printf("j");
    dimantion(&D);
    printf()
    

    if (argc < 2 || argc > 3) {
        printf("An error has occurred!\n");
        exit(1);
    }

    if (!isInteger(argv[1])){
        printf("Invalid number of clusters\n");
        exit(1);
    }
    
    K = atoi(argv[1]);
    
    if (argc == 3){ 
        iter = atoi(argv[2]);
        if (!isInteger(argv[2]) || iter <=0 || iter >= 1000){
            printf("Invalid maximum iteration!\n");
            exit(1);
        }
    }
    else iter = 200;
    
    fileParse(stdin, &data, &N, &D);

    if (N <= 0){
        printf("An error has occurred!\n");
        exit(1);
    }
    
    if (K <= 0 || K > N){
        printf("Invalid number of clusters %d %d\n", K <= 0, K > N);
        exit(1);
    }
    
    initializeClusters(data, &cluster_list, K, D);
    
    while (iter > 0 && flag){ 
        addPointsToClusters(data, cluster_list, N, D);
        flag = 1;
        curr = cluster_list;
        while (curr != NULL){
            for (i = 0; i < D; i++){
                curr->head->prev[i] = curr->head->centroid[i];
            }
            updateCentroid(curr->head, D);
            diff = calculateDistance(curr->head->prev, curr->head->centroid, D);
            if (diff > EPSILON) flag = 1;
            clearCluster(curr->head);
            curr = curr->next;
        }
        iter--;
    }

    printClusters(cluster_list, D);
    
    for (i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);   

    curr = cluster_list;
    while (curr != NULL) {
        next = curr->next;
        free(curr->head->centroid);
        free(curr->head->prev);
        free(curr->head);
        free(curr);
        curr = next;
    }
    return 0;  
}
