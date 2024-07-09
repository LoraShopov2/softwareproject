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
    CLUSTER *next;
    int size;
}CLUSTER_LIST;

typedef struct{
    int SIZE;
    double** CENTR;
    double** PREV;
    POINT_LIST POINT_LIST;
} CLUSTER;

typedef struct {
    double** HEAD;
    POINT_LIST *next;
    int size;
    }POINT_LIST;

//inserting the node
void addPoint( POINT_LIST* point_list , double** x) {
    if (point_list == NULL) {
            POINT_LIST point;
            point.HEAD = x; 
            point.next = NULL;
            point.size = 1;
            assert(point_list->HEAD != NULL);//is it ok to use asert
    }
    else{
        POINT_LIST *tmp;
        int size = point_list->size;
        tmp = &point_list;
        assert(tmp != NULL);
        *point_list->HEAD = x;
        assert(point_list->HEAD != NULL);
        *point_list->next = tmp;
        point_list->size = ++size;
    }   
}

void addCluster(CLUSTER_LIST* cluster_list , CLUSTER** x) {
    
    if (cluster_list == NULL) {
        CLUSTER_LIST cluster;
        cluster.size = 1;
        cluster.HEAD = x; 
        cluster.next = NULL;
        assert(cluster_list->HEAD != NULL);//is it ok to use asert
    }
    else{
        CLUSTER_LIST *tmp;
        int size = cluster_list->size;
        tmp = &cluster_list;
        assert(tmp != NULL);
        *cluster_list->HEAD = x;
        assert(cluster_list->HEAD != NULL);
        *cluster_list->next = tmp;
        cluster_list->size = ++size;
    }   
}

CLUSTER* createCluster(double *point){
    CLUSTER *cluster;
    cluster->CENTR = point;
    POINT_LIST *point_list;
    cluster->POINT_LIST = addPoint(point_list , point);
    return cluster;
}

void clearCluster(){}

void updateCentroid(CLUSTER *cluSTER, int D, int N){
        for (int i=0; i < D; i++){
            
            double *sum = 0;
            for( int j = 0; j < N; j++){
                double *coordinat = cluster -> POINT_LIST -> HEAD [i][j]; 
               sum += coordinat; 
            }
            sum = sum / cluster->SIZE;
            cluster -> CENTR[i] = sum;
        }
    }
}


void initiation(double **data, CLUSTER_LIST *cluster_list, int K, int N, int D){
    
    
    for(int i = 0; i < N; i++){
        if (cluster_list -> size != K){
            CLUSTER *cluster = createCluster(data[i]);
            addCluster(cluster_list, cluster);
        }
        
        else{
            double min_dist = 1.0/0.0;
            double dist = 0.0;
            CLUSTER *closest_clus;
            CLUSTER_LIST *iter = cluster_list;
            double** *point = data[i]; 
            
            while (iter != NULL) {
                dist = calculate_distance(point, iter->HEAD->CNTR);
                if (dist < min_dist){
                    min_dist = dist;
                    closest_clus = iter->HEAD;
                }
                iter = iter->next;
            }
            addPoint(closest_clus->POINT_LIST, point);
            updateCentroid()
        }
    }

}



int file_parse(FILE *file, float ***array, int *N, int *D){
    int row = 0, col = 0, SizeOfArray = 10;
    char line[MAX_LINE_LENGTH];
    int i = 0;

    // initial allocation of memory
    *array = (float **)malloc(SizeOfArray * sizeof(float *));

    while(1){
        int ch = getchar();
        if (ch == ',' || ch == ' ' || ch == '\n' || ch == EOF) {
            if (i > 0){
                line[i] = '\0';
                
                //memory allocation for mew column
                if (col == 0){
                    (*array)[row] = (float *)malloc((*D) * sizeof(float));
                }
                (*array)[row][col++] = atof(line);
                i = 0;
            }
            if (ch == '\n' || ch == EOF) {
                if (col > 0) {
                    row++;
                    if (row >= SizeOfArray) {
                        SizeOfArray *= 2;
                        *array = (float **)realloc(*array, SizeOfArray * sizeof(float *));
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
    return 1;
}


int calculate_distance(float *point_a, float *point_b,  int D){
    float sum = 0.0;
    float diff = 0.0;
    for (int i = 0; i < D; i++){
        diff = point_a[i] - point_b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

void closest_clus(double *point, CLUSTER **cluster_list, int K) {
    float min_dist = 1.0 / 0.0; // equivalent to DBL_MAX
    int index = 0;
    for (int i = 0; i < K; i++) {
        float d = calc_distance(cluster_list[i]->CENTR, point);
        if (d < min_dist) {
            min_dist = d;
            index = i;
        }
    }
    update_cluster(cluster_list[index], point);
}

void print_clusters(CLUSTER **cluster_list, int K, int D) {
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < D; j++) {
            printf("%.4f", cluster_list[i]->CENTR[j]);
            if (j < D-1) {
                printf(",");
            }
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    CLUSTER **cluster_list;
    float **data;
    
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
    
    file_parse(stdin, &data, &N, &D);
    
    if (K <= 0 || K > N){
        printf("Invalid number of clusters\n");
        return -1;
    }

    cluster_list = (CLUSTER**)malloc(K * sizeof(CLUSTER*));


    initialize_cluster(K, data, cluster_list, N, D);
    int flag = 0;

    
    while (iter > 0){ 
        for (int i = 0; i < N; i++) {
            find_closest_clus(data[i], cluster_list, K);
        }
        
        flag = 1;
        for (int i = 0; i < K; i++) {
            if (update_centroid(cluster_list[i]) > EPSILON) {
                flag = 0;
            }
            clear_cluster(cluster_list[i]);
        }
        if (!flag) {
            break;
        }
        iter--;
    }

    print_clusters(cluster_list, K, D);
    
    for (int i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);     
}
