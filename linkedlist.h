#include <stdio.h>
#include <stdlib.h>

/**
 * Struct for the Node
 */
typedef struct {
    int status; //0 if P, 1 if H
    char identifier[30];
    int base; // start index
    int limit; // size of block of memory
    struct node* before;
    struct node* next;
} node;

/**
 * Struct for the LinkedList
 */
typedef struct {
    node* head;
    node* tail;
} linkedList;

/**
 * Loads the file, and creates the nodes.
 *
 * @param fp as a file pointer
 * @param list as a linkedList struct
 */
void loadInput(FILE* fp, linkedList* list);

/**
 * Constructs the node structs.
 *
 * @param str as a char pointer
 * @return currentNode
 */
node* constructNode(char* str);

/**
 * Function to add new node structs to linkedList structs
 *
 * @param newNode as a struct node
 * @param list as a linkedList struct
 */
void addNode(node* newNode, linkedList* list);

/**
 * Prints the memory view.
 *
 * @param list as a linkedList struct
 */
void printList(linkedList * list);

/**
 * Function merges consecutive holes in the linkedList
 *
 * @param list as a linkedList Struct
 */
void mergeFreeBlocks(linkedList* list);

/**
 * Checks to make sure the bases go in order.
 *
 * @param list as a linkedList struct
 */
void checkBases(linkedList* list);

/**
 * Frees all the malloc'd nodes and the linkedList
 *
 * @param list as a linkedList Struct
 */
void freeNodes(linkedList* list);