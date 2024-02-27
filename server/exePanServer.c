#include "exePanServer.h"
#include "hsqdef.h"

//服务端执行网盘业务
int exePanServer(int netfd, MYSQL* conn, char* usrname){
    
    //获取当前系统目录
    char *sysPath = getcwd(NULL,0);
    printf("sysPath: %s\n",sysPath);
    
    int count = 0;//记录执行了多少次正确的指令
    //循环接收客户指令，完成对应任务
    while(1){

        //等待客户输入指令
        order_t order;
        orderInit(&order);
        ssize_t retRecv = recv(netfd,&order,sizeof(order),0);
        if(retRecv == 0){
            // 客户端意外断开;
            break;
        }
        switch(order.cmd){

        case LS:
            {
                break;
            }
        case CD:
            {
                break;
            }
        case PUTS:
            {
                break;
            }
        case GETS:
            {
                break;
            }

        case RM:
            {
                break;
            }
        case PWD:
            {
                break;
            }

        case MKDIR:
            {
                break;
            }
        case RMDIR:
            {
                break;

            }

        case QUIT:
            break;

        default:
            break;

        }
        if(order.cmd == QUIT){
            //退出
            break;
        }
        ++count;//执行完一次命令，业绩+1
        printf("业绩 + %d\n",count);
    }
    return 0;
}

