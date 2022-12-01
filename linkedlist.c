#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void compact(node * head) {
    //first iteration, check for how many limit in total
    node * currentNode = head;
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
    currentNode = head;
    node* onlyHoleNode = malloc(sizeof (node));
    node* previousP = malloc(sizeof (node));
    while (currentNode != NULL) {
        if (currentNode->status == 0) { //if P
            if (firstPNode == 1) {//if this is the first P node
                currentNode->before = NULL;//clean out its parent
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
                onlyHoleNode->next = NULL;//nothing behind it
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


    free(onlyHoleNode);
    free(previousP);

    printf("Operation successful");
}


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

void addNode(node* newNode, linkedList* list) {
  if (list->head == NULL) {
    list->head = newNode;
    list->tail = newNode;
  }
  list->tail->next = (struct node *) newNode;
  newNode->before = (struct node *) list->tail;
  list->tail = newNode;
}

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