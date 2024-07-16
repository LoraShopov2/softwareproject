#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 1000
#define EPSILON 0.001
#define MAX_PATH 200


// Define the node structure
typedef struct POINT_LIST{
    double *head;
    struct POINT_LIST *next;
    int size;
} POINT_LIST;

typedef struct CLUSTER{
    double *centroid;
    double *prev;
    POINT_LIST *point_list;
    int size;
} CLUSTER;

typedef struct CLUSTER_LIST{
    CLUSTER *head;
    struct CLUSTER_LIST *next;
    int size;
} CLUSTER_LIST;


//inserting the node
void addPoint( POINT_LIST **point_list , double *point) {
    POINT_LIST *tmp = (POINT_LIST *)calloc(1, sizeof(POINT_LIST));
    tmp->head = point;
    tmp->next = *point_list;
    if (*point_list == NULL) {
        tmp->size = 1;
    } else {
        tmp->size = (*point_list)->size + 1;
    }
    *point_list = tmp;
}   


void addCluster(CLUSTER_LIST **cluster_list , CLUSTER *cluster) {
    CLUSTER_LIST *tmp = (CLUSTER_LIST *)calloc(1, sizeof(CLUSTER_LIST));
    tmp->head = cluster;
    tmp->next = *cluster_list;
    *cluster_list = tmp;
}


CLUSTER *createCluster(double *point, int D){
    CLUSTER *cluster = (CLUSTER *)calloc(1, sizeof(CLUSTER));
    cluster->centroid = (double *)calloc(D, sizeof(double));
    cluster->prev = (double *)calloc(D, sizeof(double));
    int i;
    for (i = 0; i < D; i++) {
        cluster->centroid[i] = point[i];
        cluster->prev[i] = point[i];
    }
    cluster->point_list = NULL;
    cluster->size = 0;
    return cluster;
}


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

void updateCentroid(CLUSTER *cluster, int D, int N){
    int i;
    double sum;
    for (i = 0; i < D; i++){
        sum = 0.0;
        POINT_LIST *curr = cluster->point_list;
        while (curr != NULL) {
            sum += curr->head[i];
            curr = curr->next;
        }
        cluster->centroid[i] = sum / cluster->size;
    }
}

void fileParse(FILE *file, double ***array, int *N, int *D){
    int row = 0;
    int col = 0;
    int SizeOfArray = 10;
    char line[MAX_LINE_LENGTH];
    int i = 0;
    int flag = 0;
    float num;
    // initial allocation of memory
    *array = (double **)calloc(SizeOfArray, sizeof(double *));
    
    int ch;

    while((ch = getchar()) != EOF){
        if (ch == ',' || ch == '\n') {
            if (i > 0){
                line[i] = '\0';
                //memory allocation for mew row
                if (col == 0){
                     if (row >= SizeOfArray) {
                        SizeOfArray *= 2;
                        *array = (double **)realloc(*array, SizeOfArray * sizeof(double *));
                     }
                    (*array)[row] = (double *)calloc(*D, sizeof(double));
                }
                (*array)[row][col++] = atof(line);;
                i = 0;
            }
            if (ch == '\n') {
                if (flag == 0) flag++;
                if (flag == 1 && ch == '\n'){
                    *D = col;
                    flag++;
                }
                if (col > 0) {
                    row++;
                    col = 0;
                }
            }
        } 
        else line[i++] = ch;
    }
    *N = row;

}

void initializeClusters(double **data, CLUSTER_LIST **cluster_list, int K, int N, int D) {
    int i;
    for (i = 0; i < K; i++) {
        CLUSTER *cluster = createCluster(data[i], D);
        addCluster(cluster_list, cluster);
    }
}

double calculateDistance(double *point_a, double *point_b,  int D){
    double sum = 0.0;
    double diff = 0.0;
    for (int i = 0; i < D; i++){
        diff = point_a[i] - point_b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}


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


void addPointsToClusters(double **data, CLUSTER_LIST *cluster_list, int N, int D) {
    int i;
    for (i = 0; i < N; i++) {
        CLUSTER *cluster = findClosestCluster(data[i], cluster_list, D);
        addPoint(&cluster->point_list, data[i]);
        cluster->size++;
    }
}


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


int main(int argc, char *argv[]) {
    int K;
    int N;
    int D;
    int iter;
    CLUSTER_LIST *cluster_list = NULL;
    double **data;
    int flag = 0;
    double diff;
    int i;
    
    //check arguments validity
    if (argc < 2 || argc > 3) {
        printf("An error has occurred!\n");
        return -1;
    }
    K = atoi(argv[1]);
    if (argc == 3) iter = atoi(argv[2]);
    else iter = 200;
    if (iter <=0 || iter >= 1000){
        printf("Invalid maximum iteration!\n");
        return -1;
    }
    
    fileParse(stdin, &data, &N, &D);

    if (K <= 0 || K > N){
        printf("Invalid number of clusters\n");
        return -1;
    }
    
    initializeClusters(data, &cluster_list, K, N, D);
    
    while (iter > 0){ 
        addPointsToClusters(data, cluster_list, N, D);
        flag = 1;
        CLUSTER_LIST *curr = cluster_list;
        while (curr != NULL){
            for (i = 0; i < D; i++){
                curr->head->prev[i] = curr->head->centroid[i];
            }
            updateCentroid(curr->head, D, N);
            diff = calculateDistance(curr->head->prev, curr->head->centroid, D);
            if (diff > EPSILON) flag = 1;
            clearCluster(curr->head);
            curr = curr->next;
        }
        if (!flag) break;
        iter--;
    }

    printClusters(cluster_list, D);
    
    for (i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);   

    CLUSTER_LIST *curr = cluster_list;
    while (curr != NULL) {
        CLUSTER_LIST *next = curr->next;
        free(curr->head->centroid);
        free(curr->head->prev);
        free(curr->head);
        free(curr);
        curr = next;
    }
    return 0;  
}
