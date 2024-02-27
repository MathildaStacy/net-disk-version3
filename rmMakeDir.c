#include"rmMakeDir.h"
#include"pwd.h"
#include"user_dir_stack.h"
int rm(const char *Path,char *username){
     MYSQL *con = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    char com[128]={0};
    con = mysql_init(NULL);
    int preid;
    mysql_real_connect(con, "localhost", "root", "567890", "project3",
                       0, NULL, 0);
     snprintf(com,128,"SELECT fileId,type FROM files1 WHERE path='%s' and user ='%s'",Path,username);
      mysql_query(con,com);
      int preId;
      char type[32]={0};
      res = mysql_use_result(con);
      while ((row = mysql_fetch_row(res))) {
         preId=atoi(row[0]);
         strcat(type,row[1]);
     }
      bzero(com,128);
      if(strcmp(type,"dir")!=0){
         snprintf(com,sizeof(com),"UPDATE files1 set tomb = '1' where ath='/path/to/file1.txt' and user ='user1'");
      }
      else {
          snprintf(com,sizeof(com),"UPDATE files1 set tomb = '1' where preId ='%d'",preId);
      }
     int ret =  mysql_query(con,com);
     if(ret == 0 )
     {mysql_free_result(res);
         printf("rm sccess\n");
         return 0;
     }mysql_free_result(res);
return -1;

     
}
int makeDir(dirStackType *stack)
{  char name[128]={0};
    strcat(name,stack->userName);
    MYSQL *con = NULL;  
    char dir[128]={0};
    pwd(stack,dir );
    MYSQL_RES *res = NULL;  
    MYSQL_ROW row;  
    char com [128]={0};
    con = mysql_init(NULL);
    int preid;
    mysql_real_connect(con, "localhost", "root", "567890", "project3",
                       0, NULL, 0);
    snprintf(com,128,"SELECT fileId,type FROM files1 WHERE path='%s' and user ='%s'",Path,username);
         mysql_query(con,com);
         int preId=-1; 
         res = mysql_use_result(con);
            while ((row = mysql_fetch_row(res))) {
              preId=atoi (row[0]);
                printf("ID: %s\n", row[0]);
    }  char pathSql[128]={0};
            snprintf(pathSql,128,"%s/%s",Path,dirName);
           bzero(com,128);
snprintf(com,sizeof(com),"INSERT INTO files1  (filename, user, preId, path, type) values ('%s','user1',%d,'%s','dir')",dirName,preId,pathSql);
      printf("%s\n",com);     
       int ret =  mysql_query(con,com);
           if(ret== 0)
           { printf("mkdir sccess\n");
           }
           else 
           {mysql_free_result(res);
               return -1;
           }
            mysql_free_result(res);

    return 0;



}

