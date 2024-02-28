#ifndef __USER_DIR_STACK_H_
#define __USER_DIR_STACK_H_
#include <mysql/mysql.h>

typedef struct stackNodeS {
    int fileId;
    struct stackNodeS* next;
} stackNodeT;

typedef struct stackS {
    stackNodeT *head;
    stackNodeT *tail;
    int stkSize;
} stackType;

typedef struct dirStackS {
    stackType *stk;
    char userName[30];
} dirStackType;

// 初始化栈
int dirStackInit(dirStackType **dirStk);
// 检查栈是否为空
int isEmpty(dirStackType *dirStk);
// 入栈操作
int stkPush(dirStackType *dirStk, const int ele);
// 出栈操作
int stkPop(dirStackType *dirStk, int* ele);
// 获取栈顶元素
int getHead(dirStackType *dirStk, int* ele);
// 获取栈底元素
int getTail(dirStackType *dirStk, int* ele);
// 释放栈
void freeStack(dirStackType *dirStk);

//
//
int ls(int netfd, MYSQL *conn, dirStackType *dirStk);

#endif
