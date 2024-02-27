#include"rmMakeDir.h"
#include"pwd.h"
int rm(dirStackType *stack,char *fileName,MYSQL *con){
    char usrname[128]={0};
        strcat(usrname,stack->userName);
        char dir[128]={0};
        pwd(stack,dir);     
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    char com[128]={0};
    char fullPath[128]={0};
    snprintf(fullPath,128,"%s/%s",dir,fileName);
    int preid;
     snprintf(com,128,"SELECT fileId,type FROM files1 WHERE path='%s' and user ='%s'",fullPath,usrname);
      mysql_query(con,com);
      int preId;
      char type[32]={0};
      res = mysql_use_result(con);
      while ((row = mysql_fetch_row(res))) {
         preId=atoi(row[0]);
         strcat(type,row[1]);
     }mysql_free_result(res);
      bzero(com,128);
      if(strcmp(type,"dir")!=0){
         snprintf(com,sizeof(com),"UPDATE files1 set tomb = '1' where path='%s' and user ='%s'",fullPath,usrname);
      }
      else {
          snprintf(com,sizeof(com),"UPDATE files1 set tomb = '1' where preId ='%d' and  user ='%s'",preId,usrname);
      }
     int ret =  mysql_query(con,com);
     if(ret == 0 )
     {
         return 0;
     }
return -1;

     
}
int makeDir(dirStackType *stack,char *dirName,MYSQL *con)
{  char usrname[128]={0};
    strcat(usrname,stack->userName);
    char dir[128]={0};
    pwd(stack,dir );
    MYSQL_RES *res = NULL;  
    MYSQL_ROW row;  
    char com [128]={0};
    int preid;
    snprintf(com,128,"SELECT fileId,type FROM files1 WHERE path='%s' and user ='%s'",dir,usrname);
         int ret =  mysql_query(con,com);
         int preId=-1; 
         res = mysql_use_result(con);
            while ((row = mysql_fetch_row(res))) {
              preId=atoi (row[0]);
    } 
            char pathSql[128]={0};
            snprintf(pathSql,128,"%s/%s",dir,dirName);
           bzero(com,128);
snprintf(com,sizeof(com),"INSERT INTO files1  (filename, user, preId, path, type,sha1) values ('%s','%s',%d,'%s','dir','0')",dirName,usrname,preId,pathSql);
mysql_free_result(res);       
  ret =  mysql_query(con,com);
           if(ret== 0)
           { return 0 ;
           }
           else 
           {
            return -1;
           }
    return 0;
}

