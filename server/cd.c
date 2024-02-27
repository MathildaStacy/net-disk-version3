#include "cd.h"

#include "sql.h"
#include "user_dir_stack.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>


//测试
#include "pwd.h"


int cd(MYSQL *conn, dirStackType *dirStk, char *str)
{
    if(isEmpty(dirStk) && (strcmp(str, "..")) == 0)
    {
        return -1; //当前就是根目录
    }

    int pid = 0;
    getHead(dirStk, &pid);
    
    
    int file_id[1024];
    int n = findFilesByPreId(conn, pid, file_id);

    for(int i = 0; i < n; i++)
    {
        File file_s;
        bzero(&file_s, sizeof(file_s));
        getFileDataById(conn, file_id[n], &file_s);
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


    MYSQL *conn;
    sqlConnect(&conn);
    stkPush(dirStk, 1);
    
    cd(conn, dirStk, "dir2");

    char path[2048];
    bzero(path, sizeof(path));

    pwd( conn,dirStk, path);

    printf("%s\n", path);

    
}