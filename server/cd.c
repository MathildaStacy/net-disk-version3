#include "cd.h"
#include "database_struct.h"
#include "user_dir_stack.h"
#include <string.h>


int cd(dirStackType *dirStk, char *str)
{
    if(isEmpty(dirStk) && (strcmp(str, "..")) == 0)
    {
        return -1; //当前就是根目录
    }

    int pid = 0;
    getHead(dirStk, &pid);
    File file_arr[1024];
    int length = 0;
    dbListFileByParentID(pid, file_arr , &length);

    for(int i = 0; i < length; i++)
    {
        if(strcmp(file_arr[i].filename, str))
        {
            stkPush(dirStk, file_arr[i].fileId);
            break;
        }
    }

    return 0;
}