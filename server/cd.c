#include "cd.h"

#include "user_dir_stack.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>


//测试
#include "pwd.h"


int cd(dirStackType *dirStk, char *str)
{
    if(isEmpty(dirStk) && (strcmp(str, "..")) == 0)
    {
        return -1; //当前就是根目录
    }

    int pid = 0;
    getHead(dirStk, &pid);
    
    
    int file_id[1024];
    int n = findFilesByPreId(pid, file_id);

    for(int i = 0; i < n; i++)
    {
        File file_s;
        bzero(&file_s, sizeof(file_s));
        getFileDataById(file_id[n], &file_s);
        if(strcmp(str, file_s.filename) == 0)
        {
            stkPush(dirStk, file_s.fileId);
            break;
        }
    }

    return 0;
}


int main()
{
    dirStackType *dirStk;
    dirStackInit(&dirStk);

    strcpy(dirStk->userName, "user1");

    stkPush(dirStk, 1);
    
    cd(dirStk, "dir2");

    char path[2048];
    bzero(path, sizeof(path));

    pwd(dirStk, path);

    printf("%s\n", path);

    
}