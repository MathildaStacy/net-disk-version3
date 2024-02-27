#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include "sql.h"
#include <mysql/mysql.h>


int sqlConnect(MYSQL **conn)
{
    char server[]="localhost";
    char user[]="root";
    char password[]="123456";
    char database[]="user";//要访问的数据库名称
    *conn=mysql_init(NULL);
    if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
           printf("Error connecting to database:%s\n",mysql_error(*conn));
        exit(0);
    }else{
         printf("Connected...\n");
    }
    return 0;
}

void addUser(MYSQL *conn, char *name,char *salt,char *password)
{
    
    char query[200]="insert into users (username,salt,encrypted_password)values(";
    sprintf(query,"%s'%s','%s','%s')",query,name,salt,password);
    //printf("query= %s\n",query);
    int t;
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        printf("insert success\n");
    }

    mysql_close(conn);
}

int findUserByName(MYSQL *conn, char *name,char * salt, char *password)//待完成
{
    
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select salt,encrypted_password from users where username='";
    sprintf(query,"%s%s%s",query, name,"'");
    printf("%s\n",query);
    int t,ret =-1;
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        //printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            if((row=mysql_fetch_row(res))!=NULL)
            {
                if(*row[0] == 0)
                    ret = -1;
                else
                {
                    if(salt !=NULL)
                        strcpy(salt,row[0]);
                    ret = 0;
                }
            }
            //   printf("res=NULL\n");
        }else{
            printf("查询出错\n");
            exit(0);
        }
        mysql_free_result(res);
        
        
        return ret;
    }
}

void addFile(MYSQL *conn, int uid, char *name,File_info *pf)
{
    
    char query[300]="insert into files(filename,belongUserId,filetype,md5sum,filesize) values";
    sprintf(query,"%s('%s','%d','%s','%s',%d)",query,pf->filename,uid,
           pf->filetype,pf->md5sum,pf->filesize);
//    puts(query);
    
    int t = mysql_query(conn,query);
    if(t == 0){
        printf("insert success\n");}
    else{
        printf("failed!\n");
    }
    
}

char* getFilename(MYSQL *conn, int fileId) {
    

    MYSQL_RES *result;
    MYSQL_ROW row;

    // 构造 SQL 查询语句
    char query[1000];
    snprintf(query, sizeof(query), "SELECT filename FROM files WHERE fileId = %d", fileId);

    // 执行查询
    if (mysql_query(conn, query)) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    // 获取查询结果
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    // 检查是否有结果
    if (mysql_num_rows(result) == 0) {
        fprintf(stderr, "No rows found for fileId %d\n", fileId);
        mysql_free_result(result);
        mysql_close(conn);
        return NULL;
    }

    // 获取结果行
    row = mysql_fetch_row(result);
    if (row == NULL) {
        fprintf(stderr, "mysql_fetch_row() failed\n");
        mysql_free_result(result);
        mysql_close(conn);
        return NULL;
    }

    // 提取文件名
    char* filename = strdup(row[0]);

    // 释放资源并关闭数据库连接
    mysql_free_result(result);
    
    // 返回文件名
    return filename;
}

int findFilesByPreId(MYSQL *conn, int preId, int fileIds[100]) {
    

    MYSQL_RES *result;
    MYSQL_ROW row;
    // 构造 SQL 查询语句
    char query[1000];
    snprintf(query, sizeof(query), "SELECT fileId FROM files WHERE preId = %d", preId);

    // 执行查询
    if (mysql_query(conn, query)) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 0;
    }

    // 获取查询结果
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 0;
    }

    // 获取结果行数
    int numRows = mysql_num_rows(result);
    int numFiles = numRows;

    // 分配内存以存储 fileIds
    if (fileIds == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        mysql_free_result(result);
        mysql_close(conn);
        return 0;
    }

    // 读取结果并将 fileId 存储在 fileIds 数组中
    int i = 0;
    while ((row = mysql_fetch_row(result)) != NULL) {
        fileIds[i++] = atoi(row[0]);
    }
    // 释放资源并关闭数据库连接
    mysql_free_result(result);
    

    return numFiles;
}

int getFileDataById(MYSQL *conn, int fileId, File *file_s) {
    

    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[1000];
    snprintf(query, sizeof(query), "SELECT filename, user, preId, path, type, sha1 FROM files WHERE fileId = %d", fileId);

    printf("%s\n",query);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -1;
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -1;
    }

    if (mysql_num_rows(result) == 0) {
        fprintf(stderr, "No rows found for fileId %d\n", fileId);
        mysql_free_result(result);
        mysql_close(conn);
        return -1;
    }

    row = mysql_fetch_row(result);
    if (row == NULL) {
        fprintf(stderr, "mysql_fetch_row() failed\n");
        mysql_free_result(result);
        mysql_close(conn);
        return -1;
    }
    
    strcpy(file_s->filename, row[0]);
    file_s->user = atoi(row[1]);
    file_s->pre_id = atoi(row[2]);
    strcpy(file_s->absPath, row[3]);
    strcpy(file_s->type, row[4]);
    strcpy(file_s->sha1, row[5]);
    file_s->tomb = atoi(row[6]);
    printf("filename:%s\n",file_s->filename);

    mysql_free_result(result);
   

    return 0;
}    

int dbFindFileBySha1(MYSQL *conn, const char *sha1, File *file_s) {
    

    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[1000];
    snprintf(query, sizeof(query), "SELECT fileId, filename, user, preId, path, type FROM files WHERE sha1 ='%s'", sha1);

    printf("%s\n",query);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -2;
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -2;
    }

    if (mysql_num_rows(result) == 0) {
        fprintf(stderr, "No rows found for fileId %s\n", sha1);
        mysql_free_result(result);
        mysql_close(conn);
        return -1; //如果没有找到就返回-1
    }

    row = mysql_fetch_row(result);
    if (row == NULL) {
        fprintf(stderr, "mysql_fetch_row() failed\n");
        mysql_free_result(result);
        mysql_close(conn);
        return -2;
    }
    
    file_s->fileId = atoi(row[0]);
    strcpy(file_s->filename, row[1]);
    file_s->user = atoi(row[2]);
    file_s->pre_id = atoi(row[3]);
    strcpy(file_s->absPath, row[4]);
    strcpy(file_s->type, row[5]);
    file_s->tomb = atoi(row[6]);

    mysql_free_result(result);
    

    return 0;
}    


int deleteFile(int uid, int fileid)
{
    MYSQL *conn;//连接测试
    sqlConnect(&conn);

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[300]="select * from files where filename=";
    sprintf(query,"%s%d and belongUserId= '%d'",query,fileid, uid);
    //   puts(query);
    int t;
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else
    {
        //      printf("Query made...\n");
        res=mysql_use_result(conn);
        if(res)
        {
            if((row=mysql_fetch_row(res))!=NULL)
            {
                //              printf("first filename = %s\n",row[0]);
                mysql_free_result(res);
                return -1;
            }
        }else{
            printf("查询出错\n");
            exit(0);
        }
        mysql_free_result(res);
    }
    strcpy(query,"delete from files where fileid=");
    sprintf(query,"%s%d",query,fileid);
    //   puts(query);
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
    }
    mysql_close(conn);
    return 0;
}

void loginLog(const char *action,const char *name,const char *ip,const char *result)
{
    MYSQL *conn;//连接测试
    sqlConnect(&conn);
    char query[300]="insert into login(action,username,ip,result) values";
    sprintf(query,"%s('%s','%s','%s','%s')",query,action,name,ip,result);
    puts(query);
    int t;
    t=mysql_query(conn,query);
    if(!t){
           printf("insert log success\n");
    }
    else{
        printf("insert log failed!\n");
    }
    mysql_close(conn);
}

void operationLog(const char *uname,const char *action,const char *time,const char *result)
{
    MYSQL *conn;//连接测试
    sqlConnect(&conn);

    char query[300]="insert into operationLog(username,action,time,result) values";
    sprintf(query,"%s('%s','%s','%s','%s')",query,uname,action,time,result);
    puts(query);
    int t;
    t=mysql_query(conn,query);
    if(!t){
        printf("insert success\n");
        }
    else{
        printf("insert failed!\n");
    }
    mysql_close(conn);
}
