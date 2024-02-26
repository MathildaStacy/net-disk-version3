#include "worker.h"
#include "taskQueue.h"
#include "threadPool.h"
#include "exePanServer.h"
int tidArrInit(tidArr_t * ptidArr, int workerNum){
    // 申请内存 存储每个子线程的tid
    ptidArr->arr = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    ptidArr->workerNum = workerNum;
    return 0;
}
void unlock(void *arg){
    threadPool_t * pthreadPool = (threadPool_t *)arg;
    printf("unlock!\n");
    pthread_mutex_unlock(&pthreadPool->mutex);
}
void * threadFunc(void *arg){
    threadPool_t * pthreadPool = (threadPool_t *)arg;
    while(1){
        //一个循环代表了一个用户
        pthread_mutex_lock(&pthreadPool->mutex);
        int netfd;
        //pthread_cleanup_push(unlock,pthreadPool);
        while(pthreadPool->exitFlag == 0 && pthreadPool->taskQueue.queueSize <= 0){
            pthread_cond_wait(&pthreadPool->cond, &pthreadPool->mutex);
        }
        if(pthreadPool->exitFlag == 1){
            printf("I am child, I am going to exit!\n");
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }
        netfd = pthreadPool->taskQueue.pFront->netfd;
        printf("I am worker, I got a netfd = %d\n", netfd);
        deQueue(&pthreadPool->taskQueue);
        pthread_mutex_unlock(&pthreadPool->mutex);



        //连接数据库，每个线程建立自己与数据库的连接,线程不安全,上锁
        pthread_mutex_lock(&pthreadPool->mutexMysql);
        MYSQL* conn = mysql_init(NULL);
        char *host = "localhost";
        char *user = "root";
        char *password = "123456";
        char *database = "54test";
        MYSQL *ret = mysql_real_connect(conn,host,user,password,database,0,NULL,0);
        pthread_mutex_unlock(&pthreadPool->mutexMysql);
        if(ret == NULL){
            //如果数据库没有连接成功，就断开与客户端的连接
            printf("Error: %s\n", mysql_error(conn));
            close(netfd);
            continue;
        }
        //char *sql = "update S set id = 5201314 where name = 'lihua'";
        //if(mysqlfd == NULL){
        //}
        //int qret = mysql_query(mysqlfd,sql);
        //if(qret == -1){
        //    printf("修改失败\n");
        //}
        //printf("connect over mysql\n");

        //登陆操作
        char *usrname = "huangshuiqing";
                


        //执行网盘业务
        exePanServer(netfd, conn, usrname);
        printf("I am worker, netfd = %d, over\n",netfd);
        //子线程结束业务，用户退出，断开和用户的连接，然后继续等待下一个用户的连接
        mysql_close(conn);
        close(netfd);
    }
}
