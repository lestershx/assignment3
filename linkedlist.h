typedef struct {
    int status; //0 if P, 1 if H
    char identifier[30];
    int base; // start index
    int limit; // size of block of memory
    struct node* before;
    struct node* next;
} node;

typedef struct {
    node* head;
    node* tail;
} linkedList;

void loadInput(FILE* fp, linkedList* list);

node* constructNode(char* str);

void addNode(node* newNode, linkedList* list);

void printList(linkedList * list);