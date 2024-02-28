#include "user_dir_stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int dirStackInit(dirStackType **dirStk) {
    *dirStk = (dirStackType *)calloc(1, sizeof(dirStackType));
    if (*dirStk == NULL) {
        return 1;
    }

    (*dirStk)->stk = (stackType *)calloc(1, sizeof(stackType));
    if ((*dirStk)->stk == NULL) {
        free(*dirStk);  // 释放已分配的dirStackType指针
        return 1;
    }

    return 0;
}

int isEmpty(dirStackType *dirStk) {
    return dirStk->stk->stkSize == 0;
}

//不成功返回1，成功返回0，本函数会动态内存分配克隆一个ele字符串的副本
int stkPush(dirStackType *dirStk, int ele) {
    stackNodeT *new_node = (stackNodeT *)calloc(1, sizeof(stackNodeT));
    if(new_node == NULL) {
        return 1;
    }

    new_node->fileId = ele; // 使用strdup克隆一个字符串副本
   

    if(dirStk->stk->stkSize == 0) {
        dirStk->stk->head = new_node;
        dirStk->stk->tail = new_node;
    } else {
        new_node->next = dirStk->stk->head;
        dirStk->stk->head = new_node;
    }
    dirStk->stk->stkSize++;

    return 0;
}

int stkPop(dirStackType *dirStk, int* ele) {
    if(isEmpty(dirStk)) {
        return 1;
    }

    stackNodeT *node_to_pop = dirStk->stk->head;
    dirStk->stk->head = dirStk->stk->head->next;
    dirStk->stk->stkSize--;

    if(dirStk->stk->stkSize == 0) {
        dirStk->stk->tail = NULL;
    }

    *ele = node_to_pop->fileId; // Assign the string to the output parameter
    free(node_to_pop);

    return 0; 
}

int getHead(dirStackType *dirStk, int* ele) {
    if (isEmpty(dirStk)) {
        return 1;
    }
    *ele = dirStk->stk->head->fileId;
    return 0;
}

int getTail(dirStackType *dirStk, int * ele) {
    if (isEmpty(dirStk)) {
        return 1;
    }
    *ele = dirStk->stk->tail->fileId;
    return 0;
}

void freeStack(dirStackType *dirStk) {
    stackNodeT* current = dirStk->stk->head;
    while(current != NULL) {
        stackNodeT* next = current->next;
        
        free(current);
        current = next;
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
