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
    int size;
    float** centroid;
    float** prev;
    CLUS_LIST clus_list ;

} CLUSTER;

typedef struct {
    float** point;
     POINT *next;
}POINT;
typedef struct {
    POINT head;
}CLUS_LIST;

//inseting the node
void appendNode( CLUS_LIST* clus_list , float** x ) {
    if (clus_list==NULL) {
            POINT point;
            point 
        clus_list.head = point;
        assert(clus_list!=NULL);
    }
    else{
        ** tmp  = &clus_list;
        clus_list = &point;
        clus_list- >next = tmp;
    

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}
}

,L,L,L,LLMMMLM
 MJL;' 
 \ NM'





