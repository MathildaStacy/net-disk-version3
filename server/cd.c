#include "cd.h"

#include "user_dir_stack.h"
#include <string.h>
#include <strings.h>


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