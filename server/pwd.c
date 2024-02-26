#include "pwd.h"
#include <stdio.h>
#include <string.h>

int pwd(dirStackType *dirStk, char *str) {
    if (isEmpty(dirStk)) {
        sprintf(str, "/%s", dirStk->userName);  // 如果目录栈为空，只返回根路径
        return 0;
    }

    // 为了从栈底到栈顶访问，我们可能需要先反转栈或使用临时数组/栈存放元素
    // 这里简化处理，假设用一个足够大的数组模拟这个过程
    // 注意，这种方法可能存在栈非常大时的内存问题
    int tempIds[128]; // 假设栈的深度不会超过128
    int top = 0;
    
    // 临时保存原始栈信息
    while (!isEmpty(dirStk)) {
        stkPop(dirStk, &tempIds[top++]);
    }

    // 重建原始栈的同时构建路径
    strcpy(str, ""); // 清空字符串
    strcat(str, "/");
    strcat(str, dirStk->userName);

    for (int i = top - 1; i >= 0; i--) {
        File file;
        int ret = getFileDataById(tempIds[i], &file);
        if(ret != -1) {
            strcat(str, "/");
            strcat(str, file.filename);
            stkPush(dirStk, tempIds[i]); // 还原栈结构
        } else {
            // 处理通过ID找不到文件的异常情况
            // 具体的错误处理根据需求定义
        }
    }

    return 0;
}