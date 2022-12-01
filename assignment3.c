#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int main() {
  linkedList* listPtr = malloc(sizeof(linkedList));
  listPtr->head = NULL;
  int menuOption = 0;
  char fileName[100];

  while (menuOption != 5) {
    printf("\n1. load an input file\n2. merge holes\n3. compact memory\n4. print memory view\n5. Exit the program\n");
    scanf("%d", &menuOption);
    switch(menuOption) {
      case 1:
        printf("1. Type the file name: ");
        scanf("%s", fileName);

        FILE *fp;
        fp = fopen(fileName, "r");
        loadInput(fp, listPtr);
        break;
      case 2:
        break;
      case 3:
        compact(listPtr->head);
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
