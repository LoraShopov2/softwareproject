#include <stdio.h>
#include <stdlib.h>


#define MAX_LINE_LENGTH 1000

int K, iter, N, D;
typedef struct{
    void* 
    } DATA;

int main(int argc, char *argv[]) {
    
    float ***data;
    
    if (argc < 3 || argc > 4) {
        printf("An error has occurred!\n");
        return -1;
    }
    int K = atoi(argv[1]);
    if (argc == 4) iter = atoi(argv[2]);
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
                    col = 0;
                }
                if (ch == EOF) break;
            }
        } else {
            line[i++] = ch;
        }
    }
    *N = row;
    return;
}


// Define the node structure
typedef struct{
    int SIZE;
    float** CENTR;
    float** PREV;
    POINT_LIST POINT_LIST;

} CLUSTER;

typedef struct {
    float** HEAD;
     POINT_LIST *next;
}POINT_LIST;


//inseting the node
void addPoint( POINT_LIST* clus_list , float** x) {
    if (clus_list==NULL) {
            POINT_LIST point;
            point.HEAD = x; 
        point.next = NULL;
        assert(clus_list->HEAD != NULL);
    }
    else{
        POINT_LIST *tmp;
        tmp = & clus_list;
        assert(tmp != NULL);
        *clus_list->HEAD = x;
        assert(clus_list->HEAD != NULL);
        *clus_list->next = tmp;



}

     CLUSTER* cluster_list = (struct CLUSTER *)malloc(sizeof(struct CLUSTER))
}
    void initiation(float **data, CLUSTER** cluster_list, int K, int N, int D){
        cluster_ptr = (struct CLUSTER *)malloc(sizeof(struct CLUSTER))
            for(int i = 0; i < N; i++){
                for(int j = 0; j < D; j++){
                    if (i < K) {
                        if (**cluster_list[i].CENTR == NULL){

                        } 
                    }
                   if cluster_liST[i] 
                }
            }
    }





