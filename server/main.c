#include "worker.h"
#include "taskQueue.h"
#include "threadPool.h"
int exitPipe[2];
void handler(int signum){
    printf("signum = %d\n", signum);
    write(exitPipe[1],"1",1);
}
int main(int argc, char *argv[])
{
    // ./server 192.168.72.128 1234 4
    ARGS_CHECK(argc,4);

    pipe(exitPipe);
    if(fork()!= 0){
        close(exitPipe[0]);
        signal(SIGUSR1,handler);
        wait(NULL);
        printf("Parent is going to exit!\n");
        exit(0);
    }
    // 只有子进程 才能创建线程池
    close(exitPipe[1]);
    threadPool_t threadPool;
    threadPoolInit(&threadPool, atoi(argv[3]));
    makeWorker(&threadPool);

    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);

    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,exitPipe[0]);
    while(1){
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd,readySet,1024,-1);
        for(int i = 0; i < readyNum; ++i){
            if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                printf("I got 1 task!\n");
                // 分配任务
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue,netfd);
                printf("I am master, I send a netfd  = %d\n", netfd);
                pthread_cond_signal(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
            }
            //else if(readySet[i].data.fd == exitPipe[0]){
            //    // 线程池知道信号来了
            //    printf("threadPool is going to exit!\n");
            //    //for(int j = 0; j < threadPool.tidArr.workerNum; ++j){
            //    //    pthread_cancel(threadPool.tidArr.arr[j]);
            //    //    
            //    //}
            //    pthread_mutex_lock(&threadPool.mutex);
            //    threadPool.exitFlag = 1;
            //    pthread_cond_broadcast(&threadPool.cond);
            //    pthread_mutex_unlock(&threadPool.mutex);
            //    for(int j = 0; j < threadPool.tidArr.workerNum; ++j){
            //        pthread_join(threadPool.tidArr.arr[j],NULL);
            //    }
            //    printf("main thread is going to exit!\n");
            //    exit(0);
            //}
        }
    }
    return 0;
}

