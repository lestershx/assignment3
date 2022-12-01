#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

/**
 * This function compacts together the Processes and puts the Holes together at the end
 * of the list.
 *
 * @param list as a linkedList struct
 */
void compact(linkedList * list) {
    if (list->head == NULL) {
        perror("Error: File has not been uploaded, please upload a file.\n");
        exit(-1);
    }
    //first iteration, check for how many limit in total
    node * currentNode = list->head;
    int spaceOccu = 0;
    while (currentNode->next != NULL) {
        if (currentNode->status == 0) {
            spaceOccu += currentNode->limit;
        }
        currentNode = (node *)currentNode->next;
    }

    //second iteration
    //P pointer set at 0 and H pointer set at spaceOccu
    //if P node, move base to 0, set P pointer to pointer(0 at first)+limit
    //if H pointer, move base to spaceOccu, set H pointer to pointer(spaceOccu at first)+limit
    int pPtr = 0;
    int hPtr = spaceOccu;
    int firstHNode = 1;
    int firstPNode = 1;
    currentNode = list->head;
    node* onlyHoleNode = malloc(sizeof (node));
    node* previousP = malloc(sizeof (node));
    while (currentNode != NULL) {
        if (currentNode->status == 0) { //if P
            if (firstPNode == 1) {//if this is the first P node
                currentNode->before = NULL;//clean out its parent
                list->head = currentNode;
                firstPNode = 0;
            } else {
                currentNode->before = (struct node *)previousP;//set its parent to the previous P
                previousP->next = (struct node *) currentNode;//set previous P node's child
            }
            currentNode->base = pPtr;//set the base no. to the pointer location
            pPtr = pPtr + currentNode->limit;//move pointer by the limit
            previousP = currentNode;//make this node the previous P for later use
        }
        if (currentNode->status == 1) {//if H
            if (firstHNode == 1) {//if it's first H node, set this node up to be added later
                currentNode->base = hPtr;//the base is at hPtr
                onlyHoleNode = currentNode;//becomes the only H node
                list->tail = currentNode;
                if (previousP != NULL) {//if there's a previous P
                    onlyHoleNode->before = (struct node *)previousP;//set its parent to the previous P
                } else {
                    onlyHoleNode->before = NULL;
                }
                firstHNode = 0;
            } else {//not the first H node
                onlyHoleNode->limit += currentNode->limit;//go to the first H node, and add to the limit
                onlyHoleNode->before = (struct node *) previousP;//set the last P node to parent of onlyHoleNode
                previousP->next = (struct node *) onlyHoleNode;
            }
        }
        currentNode = (node *)currentNode->next;
    }

    onlyHoleNode->next = NULL;//make sure the node isn't linking to a child
    onlyHoleNode->before = (struct node *) previousP;//make sure the h node is linked to the last p node

    printf("Operation successful\n");
}

/**
 * Constructs the node structs.
 *
 * @param str as a char pointer
 * @return currentNode
 */
node* constructNode(char* str) {
  node* currentNode = malloc(sizeof(node));
  char* token = strtok(str, " \n");
  int i = 1;
  while (token != NULL) {
    //Identifier token
    if (i==1) {
      char identifier = token[0];
      strcpy(currentNode->identifier, token);
      if (identifier == 'P') {
        currentNode->status = 0;
      } else if (identifier == 'H') {
        currentNode->status = 1;
      } else {
        printf("Error: Invalid memory identifier!");
        exit(-1);
      }
    }
    //Base token
    if (i == 2) {
      currentNode->base = atoi(token);
    }
    //Limit token
    if (i == 3) {
      currentNode->limit = atoi(token);
    }
    if (i >= 4) {
      printf("Error: Unexpected token!");
      exit(-1);
    }
    token = strtok(NULL, " \n");
    i++;
  }
  currentNode->before = NULL;
  currentNode->next = NULL;
  return currentNode;
}

/**
 * Inserts new nodes into the list in order of base value.
 *
 * NOTE: Adapted from the Insert Sort into Doubly Linked List from Geeks-for-Geeks
 * https://www.geeksforgeeks.org/insert-value-sorted-way-sorted-doubly-linked-list/
 *
 * @param newNode node pointer to the node to be inserted
 * @param list list pointer to the LinkedList
 */
void addNode(node* newNode, linkedList* list) {
  node* currentNode = list->head;
  if (currentNode == NULL) {
    list->head = newNode;
    list->tail = newNode;
  }
  else if (currentNode->base >= newNode->base) {
    newNode->next = (struct node *) currentNode;
    currentNode->before = (struct node *) newNode;
    list->head = newNode;
  } else {
    node* aheadNode = (node *) currentNode->next;
    while (currentNode->next != NULL && aheadNode->base < newNode->base) {
      currentNode = (node *) currentNode->next;
      aheadNode = (node *) currentNode->next;
    }
    newNode->next = currentNode->next;
    if (currentNode->next != NULL) {
      node* newNext = (node *) newNode->next;
      newNext->before  = (struct node *) newNode;
    }
    currentNode->next = (struct node *) newNode;
    newNode->before = (struct node *) currentNode;
  }
}

/**
 * Function merges consecutive holes in the linkedList.
 *
 * @param list as a linkedList Struct
 */
void mergeFreeBlocks(linkedList* list) {
  if (list->head == NULL) {
    printf("Error: File has not been uploaded, please upload a file.\n");
    return;
  }
  node *currentNode = list->head; //Assign head to node
  while (currentNode != NULL && currentNode->next != NULL) { //While node is not null
    node *nextNode = (node *) currentNode->next; //Assign next node
    if (currentNode->status == 1 && nextNode->status == 1) { //If this node and next node are holes
      currentNode->limit += nextNode->limit; //Add the limits together
      currentNode->next = nextNode->next; //Skip the next node
    } else {
      currentNode = (node *) currentNode->next;
    }
  }
  printf("Operation successful\n");
}

/**
 * Loads the file, and creates the nodes.
 *
 * @param fp as a file pointer
 * @param list as a linkedList struct
 */
void loadInput(FILE* fp, linkedList* list){
  char buffer[100];

  if (fp == NULL) {
    printf("Error: File not found!");
    exit(-1);
  }
  while (fgets(buffer, 100, fp) != NULL) {
    node* memNode = constructNode(buffer);
    addNode(memNode, list);
  }
}

/**
 * Prints the memory view.
 *
 * @param list as a linkedList struct
 */
void printList(linkedList * list) {
  node* currentNode = list->head;
  if (currentNode == NULL) {
    printf("Error: List is empty!");
    exit(-1);
  }
  int nodeNum = 1;
  while (currentNode != NULL) {
    printf("Node %d: %s", nodeNum, currentNode->identifier);
    if (currentNode->status == 1) {
      printf(" (Hole)");
    }
    printf(", base = %d, limit = %d\n", currentNode->base, currentNode->limit);
    currentNode = (node*) currentNode->next;
    nodeNum++;
  }
  printf("\n");
}

/**
 * Checks to make sure the bases go in order.
 *
 * @param list as a linkedList struct
 */
void checkBases(linkedList* list) {
  node* currentNode = list->head;
  if (currentNode == NULL) {
    printf("Error: List is empty!");
    exit(-1);
  }
  while (currentNode != NULL) {
    int nextBase = currentNode->base + currentNode->limit;
    node* nextNode = (node *) currentNode->next;
    if (nextNode != NULL && nextNode->base != nextBase) {
      printf("Error: Base numbers do not match!");
      exit(-1);
    }
    currentNode = (node*) currentNode->next;
  }
  printf("Operation successful\n");
}

/**
 * Frees all the malloc'd nodes and the linkedList.
 *
 * @param list as a linkedList Struct
 */
void freeNodes(linkedList* list) {
  if (list->head == NULL) {
    printf("Error: List is empty!");
  }

  node *ptr = NULL;
  ptr = (node *) list->head->next;
  while (ptr != NULL) {
    free(ptr->before);
    ptr = (node *) ptr->next;
  }

  free(ptr);
  free(list);
}