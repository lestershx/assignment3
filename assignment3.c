#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

/**
 * File: assignment3.c
 *
 * Author: Lester Shun, Caroline Lin, Betty Nguyen, Izabelle Datayan
 * Date: November 30, 2022
 * Course: COMP 2510
 *
 * Summary of File:
 *
 *  This program takes works on a real life application of LinkedLists. We take in a user
 *  input and can do 4 options. The user can upload a file, they can merge the holes,
 *  compact memory, and print memory view.
 *
 */

/**
 * Main function, runs the program.
 *
 * @return 0
 */
int main() {
  linkedList* listPtr = malloc(sizeof(linkedList));
  listPtr->head = NULL;
  int menuOption = 0;
  char fileName[100];

  while (menuOption != 5) {
    printf("1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. Exit the program\n");
    scanf("%d", &menuOption);
    switch(menuOption) {
      case 1:
        printf("1. Type the file name: ");
        scanf("%s", fileName);

        FILE *fp;
        fp = fopen(fileName, "r");
        loadInput(fp, listPtr);
        checkBases(listPtr);
        break;
      case 2:
        mergeFreeBlocks(listPtr);
        break;
      case 3:
        break;
      case 4:
        printList(listPtr);
        break;
      case 5:
        exit(0);
      default:
        break;
    }
  }
  return 0;
}
