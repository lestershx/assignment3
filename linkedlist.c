#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

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
 * Function merges consecutive holes in the linkedList
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
  printf("operation successful\n");
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
  printf("operation successful\n");
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
  printf("operation successful\n");
}

/**
 * Frees all the malloc'd nodes and the linkedList
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