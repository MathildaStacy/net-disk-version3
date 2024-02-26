#include "user_dir_stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int dirStackInit(dirStackType **dirStk) {
    *dirStk = (dirStackType *)malloc(sizeof(dirStackType));
    if (*dirStk == NULL) return -1;
    (*dirStk)->stk = (stackType *)malloc(sizeof(stackType));
    if ((*dirStk)->stk == NULL) return -1;
    (*dirStk)->stk->head = NULL;
    (*dirStk)->stk->stkSize = 0;
    return 0;
}

int isEmpty(dirStackType *dirStk) {
    return dirStk->stk->stkSize == 0;
}

int stkPush(dirStackType *dirStk, const int ele) {
    stackNodeT *newNode = (stackNodeT *)malloc(sizeof(stackNodeT));
    if (newNode == NULL) return -1;
    newNode->fileId = ele;
    newNode->next = dirStk->stk->head;
    dirStk->stk->head = newNode;
    dirStk->stk->stkSize++;
    return 0;
}

int stkPop(dirStackType *dirStk, int* ele) {
    if (isEmpty(dirStk)) return -1;
    stackNodeT *temp = dirStk->stk->head;
    *ele = temp->fileId;
    dirStk->stk->head = dirStk->stk->head->next;
    free(temp);
    dirStk->stk->stkSize--;
    return 0;
}

int getHead(dirStackType *dirStk, int* ele) {
    if (isEmpty(dirStk)) return -1;
    *ele = dirStk->stk->head->fileId;
    return 0;
}

void freeStack(dirStackType *dirStk) {
    while (!isEmpty(dirStk)) {
        int ele;
        stkPop(dirStk, &ele);
    }
    free(dirStk->stk);
    free(dirStk);
}

/*
int main() {
    dirStackType *dirStack;
    int status = dirStackInit(&dirStack);
    if (status == -1) {
        printf("Failed to initialize dirStack.\n");
        return 1;
    }

    // Test operations
    stkPush(dirStack, 10);
    stkPush(dirStack, 20);
    stkPush(dirStack, 30);

    int head;
    getHead(dirStack, &head); // Should return 30, as it is the last pushed = top of stack
    printf("Head/Top: %d\n", head);

    int popped;
    while (!isEmpty(dirStack)) {
        stkPop(dirStack, &popped);
        printf("Popped: %d\n", popped); 
        // Should print in reverse order they were added: 30, 20, 10, demonstrating LIFO behavior
    }

    // Free resources
    freeStack(dirStack);

    return 0;
}
*/