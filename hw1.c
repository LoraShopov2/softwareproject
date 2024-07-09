#include <stdio.h>
#include <stdlib.h>

const int D;
const int K;
const int N;


// Define the node structure
struct Node{
    int data;
    struct Node* next;
};


//creating a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Memory allocation error\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

//inseting the node
void appendNode(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (!newNode) return;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
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
