#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 100
#define EPSILON 0.001
#define MAX_PATH 200

int K, iter, N, D;

// Define the node structure
typedef struct{
    CLUSTER *HEAD;
    CLUSTER *NEXT;
    int size;
}CLUSTER_LIST;

typedef struct{
    int size;
    double** CENTEROID;
    double** PREV;
    POINT_LIST POINT_LIST;
} CLUSTER;

typedef struct {
    double** HEAD;
    POINT_LIST *NEXTt;
    int size;
}POINT_LIST;

//inserting the node
void addPoint( POINT_LIST* point_list , double** x) {
    if (point_list == NULL) {
            POINT_LIST point;
            point.HEAD = x; 
        point.NEXT = NULL;
        point.size = 1;
    }
    else{
        POINT_LIST *tmp;
        int size = point_list->size;
        tmp = &point_list;
        *point_list->HEAD = x;
        *point_list->NEXT = tmp;
        point_list->size = ++size;
    }   
}

void addCluster(CLUSTER_LIST* cluster_list , CLUSTER** x) {
    
    if (cluster_list == NULL) {
        CLUSTER_LIST cluster;
        cluster.size = 1;
        cluster.HEAD = x; 
        cluster.NEXT = NULL;
        assert(cluster_list->HEAD != NULL);//is it ok to use asert
    }
    else{
        CLUSTER_LIST *tmp;
        int size = cluster_list->size;
        tmp = &cluster_list;
        assert(tmp != NULL);
        *cluster_list->HEAD = x;
        assert(cluster_list->HEAD != NULL);
        *cluster_list->NEXT = tmp;
        cluster_list->size = ++size;
    }   
}

CLUSTER* createCluster(double *point){
    CLUSTER *cluster;
    cluster->CENTEROID = point;
    POINT_LIST *point_list;
    cluster->POINT_LIST = addPoint(point_list , point);
    return cluster;
}

void clearCluster(Cluster *cluster) {
    PointList *current = cluster->points;
    PointList *next;
    while (current != NULL) {
        next = current->NEXT
        free(current);
        current = next;
    }
    cluster->points = NULL;
    cluster->size = 0;
}

void updateCenteroid(CLUSTER *cluster, int D, int N){
    for (int i=0; i < D; i++){
        double *sum = 0;
        for( int j = 0; j < N; j++){
            double *coordinate = cluster->POINT_LIST->HEAD [i][j]; 
            sum += coordinate; 
        }
        sum = sum / cluster->size;
        cluster -> CENTEROID[i] = sum;
    }
}


void initiation(double **data, CLUSTER_LIST *cluster_list, int K, int N, int D){
    for(int i = 0; i < N; i++){
        if (cluster_list->size != K){
            CLUSTER *cluster = createCluster(data[i]);
            addCluster(cluster_list, cluster);
        }
        else{
            double min_dist = INFINITY;
            double dist = 0.0;
            CLUSTER *closest_clus;
            CLUSTER_LIST *iter = cluster_list;
            double** *point = data[i]; 
            while (iter != NULL) {
                dist = calculateDistance(point, iter->HEAD->CENTEROID);
                if (dist < min_dist){
                    min_dist = dist;
                    closest_clus = iter->HEAD;
                }
            }
            addPoint(closest_clus->POINT_LIST, point);
            updateCenteroid();
            iter = iter->next;
        }
    }        
}


void fileParse(FILE *file, double ***array, int *N, int *D){
    int row = 0, col = 0, SizeOfArray = 10;
    char line[MAX_LINE_LENGTH];
    int i = 0;

    // initial allocation of memory
    *array = (double **)malloc(SizeOfArray * sizeof(double *));

    while(1){
        int ch = getchar();
        if (ch == ',' || ch == ' ' || ch == '\n' || ch == EOF) {
            if (i > 0){
                line[i] = '\0';
                
                //memory allocation for mew column
                if (col == 0){
                    (*array)[row] = (double *)malloc((*D) * sizeof(double));
                }
                (*array)[row][col++] = atof(line);
                i = 0;
            }
            if (ch == '\n' || ch == EOF) {
                if (col > 0) {
                    row++;
                    if (row >= SizeOfArray) {
                        SizeOfArray *= 2;
                        *array = (double **)realloc(*array, SizeOfArray * sizeof(double *));
                    }
                    if (ch == '\n') col = 0;
                }
                if (ch == EOF) break;
            }
        } else {
            line[i++] = ch;
        }
    }
    *N = row;
    *D = col;
}


int calculateDistance(double *point_a, double *point_b,  int D){
    double sum = 0.0;
    double diff = 0.0;
    for (int i = 0; i < D; i++){
        diff = point_a[i] - point_b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

void findClosestCluster(double *point, CLUSTER **cluster_list, int D) {
    double minDist = INFINITY;
    Cluster *closestCluster = NULL;
    ClusterList *current = clusterList;

    while (current != NULL) {
        double dist = calculateDistance(point, current->HEAD->CENTEROID, D);
        if (dist < minDist) {
            minDist = dist;
            closestCluster = current->head;
        }
        current = current->next;
    }

    addPoint(&closestCluster->points, point);
    closestCluster->size++;
}


void printClusters(ClusterList *clusterList, int D) {
    ClusterList *current = clusterList;
    while (current != NULL) {
        for (int j = 0; j < D; j++) {
            printf("%.4f", current->HEAD->CENTEROID[j]);
            if (j < D - 1) {
                printf(",");
            }
        }
        printf("\n");
        current = current->NEXT;
    }
}


int main(int argc, char *argv[]) {

    CLUSTER **cluster_list;
    double **data;
    
    if (argc < 2 || argc > 3) {
        printf("An error has occurred!\n");
        return -1;
    }
    int K = atoi(argv[1]);
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

    cluster_list = (CLUSTER**)malloc(K * sizeof(CLUSTER*));


    initialization(K, data, cluster_list, N, D);
    int flag = 0;

    
    while (iter > 0){ 
        for (int i = 0; i < N; i++) {
            findClosestCluster(data[i], cluster_list, D);
        }
        
        flag = 1;
        for (int i = 0; i < K; i++) {
            if (updateCenteroid(cluster_list[i]) > EPSILON) {
                flag = 0;
            }
            clearCluster(cluster_list[i]);
        }
        if (!flag) {
            break;
        }
        iter--;
    }

    printClusters(cluster_list, K, D);
    
    for (int i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);     
}
