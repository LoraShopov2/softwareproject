#include <stdio.h>
#include <stdlib.h>
int main(){
int D;
int K;
int N;
typedef double coordinats[D];

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
