#include "exePanServer.h"
#include "hsqdef.h"
#include "user_dir_stack.h"
#include "cd.h"
#include "pwd.h"
#include "puts.h"
#include "fbr_gets_and_puts.h"
#include "rmMakeDir.h"
//服务端执行网盘业务
int exePanServer(int netfd, MYSQL* conn, char* usrname){
    
    dirStackType * dirstack;
    dirStackInit(& dirstack);
    strcpy(dirstack->userName, usrname);  
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
                ls(netfd, conn, dirstack);
                break;
            }
        case CD:
            {
                int ret = cd(conn,dirstack,order.parameters[0]);
                send(netfd, &ret, sizeof(int),0);
                break;
            }
        case PUTS:
            {
                commandPuts_S(conn, dirstack, netfd);
                break;
            }
        case GETS:
            {
                printf("exepan.c |||| filename = %s\n", order.parameters[0]);
                server_send(conn,dirstack,order.parameters[0],netfd);
                break;
            }

        case RM:case RMDIR:
            {
               int ret = rm(dirstack, order.parameters[0],conn);
               send(netfd, &ret, sizeof(int),0);
                break;
            }
        case PWD:
            {
                char buf[1024] = {0};
                pwd(conn, dirstack, buf);
                send(netfd, buf, 1024, 0);
                break;
            }

        case MKDIR:
            {   
                int ret = makeDir(dirstack, order.parameters[0],conn);
                printf("makeDir over\n");
                send(netfd, &ret, sizeof(int),0);
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

