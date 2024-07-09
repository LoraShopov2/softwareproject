#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_LINE_LENGTH 1000
#define EPSILON 0.001
#define MAX_PATH 200



int main(int argc, char *argv[]) {
    int K, iter, N, D;
    float **data;
    
    if (argc < 3 || argc > 4) {
        printf("An error has occurred!\n");
        return -1;
    }
    int K = atoi(argv[1]);
    if (argc == 4) iter = atoi(argv[2]);
    else iter = MAX_PATH;
    
    if (iter <=0 || iter >= 1000){
        printf("Invalid maximum iteration!\n");
        return -1;
    }
    
    file_parse(stdin, &data, &N, &D);

    if (K <= 0 || K > N){
        printf("Invalid number of clusters\n");
        return -1;
    }



    typedef double coordinats[D];
    

    for (int i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);

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
    return 0;
}



void print_clusters(CLUSTER **cluster_list, int K, int D) {
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < D; j++) {
            printf("%.4f", cluster_list[i]->centroid[j]);
            if (j < D-1) {
                printf(",");
            }
        }
        printf("\n");
    }
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




// Define the node structure
typedef struct {
    coordinats cor;
    struct POINT* next;
} POINT;


typedef  struct  linked_list   CLUS_LIST;
 typedef  CLUS_LIST* LINK;
POINT* createNode(coordinats point)
{
    POINT a;
    a.cor = point;
    a.next = NULL;
}

//inseting the node
void appendNode( LINK CLUS_LIST, coordinats point) {
    LINK newPoint = createNode(point);
    if (head==NULL) {
        head = ( LINK )malloc( sizeof(POINT) );
        assert( head!=NULL);
    };
    else if{
        
    }
    if (head.coordinats == NULL) {
        head = &newPoint;
        return he;
    }

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}
}







typedef linked_list CLUST_LIST;
typedef double[D] POINT;

struct linked_list{
DATA d;
struct linked_list *next;


jsj

};
struct CLUSTER{

}
