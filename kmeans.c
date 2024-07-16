#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 1000
#define EPSILON 0.001
#define MAX_PATH 200


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

void fileParse(FILE *file, double ***array, int *N, int *D) {
    int row = 0;
    int col;
    char line[MAX_LINE_LENGTH];
    char *ch;
    char *start;

    *array = (double **)calloc(1, sizeof(double *));
    if (*array == NULL) {
        printf("An error has occurred!\n");
        exit(1);
    }
    
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        *array = (double **)realloc(*array, (row + 1) * sizeof(double *));
        if (*array == NULL) {
            printf("An error has occurred!\n");
            exit(1);
        }
        
        (*array)[row] = (double *)calloc(100, sizeof(double));
        if ((*array)[row] == NULL) {
            printf("An error has occurred!\n");
            exit(1);
        }
        col = 0;
        ch = line;
        start = ch;
        while (*ch != '\0') {
            if (*ch == ',' || *ch == '\n') {
                *ch = '\0'; 
                (*array)[row][col++] = atof(start);
                start = ch + 1;
            }
            ch++;
        }
        if (row == 0) {
            *D = col;
        }
        row++;
    }
    *N = row;
}

void initializeClusters(double **data, CLUSTER_LIST **cluster_list, int K, int D) {
    int i;
    for (i = 0; i < K; i++) {
        CLUSTER *cluster = createCluster(data[i], D);
        addCluster(cluster_list, cluster);
    }
}

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
    int D=1;
    int iter;
    CLUSTER_LIST *cluster_list = NULL;
    double **data;
    int flag = 0;
    double diff;
    int i;
    CLUSTER_LIST *curr;
    CLUSTER_LIST *next;
    

    if (argc < 2 || argc > 3) {
        printf("An error has occurred!\n");
        exit(1);
    }
    K = atoi(argv[1]);
    if (argc == 3) iter = atoi(argv[2]);
    else iter = 200;
    if (iter <=0 || iter >= 1000){
        printf("Invalid maximum iteration!\n");
        exit(1);
    }
    
    fileParse(stdin, &data, &N, &D);

    if (K <= 0 || K > N){
        printf("Invalid number of clusters\n");
        exit(1);
    }
    
    initializeClusters(data, &cluster_list, K, D);
    
    while (iter > 0){ 
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
        if (!flag) break;
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
