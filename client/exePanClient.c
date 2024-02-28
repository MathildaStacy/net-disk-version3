#include "exePanClient.h"
#include "hsqdef.h"
#include "analyOrder.h"
#include "puts.h"
#include "fbr_gets_and_puts.h"
int exePanClient(int sockfd, char*usrname){

    char bufPrintf[1024] = {0};//保存当前命令行提示符信息
    sprintf(bufPrintf, "%s%s",usrname,":~$ ");
    while(1){

        //打印命令行提示信息
        displayCmdLine(bufPrintf);
        //向客户端发送指令 
        char buf[1024] = {0};
        memset(buf, 0, sizeof(buf));
        ssize_t retStdin = read(STDIN_FILENO, buf, sizeof(buf));
        order_t order;
        orderInit(&order);//初始化指令结构体
        int ret = analyOrder(buf, &order);
        if(ret == -1){
            //指令输入错误，进入下一次循环，继续等待用户输入
            continue;
        }
        printf("cmd = %d\n",order.cmd);//打印当前指令宏
        for(int i = 0; i < order.num; ++i){
            //循环打印当前参数和参数大小
            printf("parameter = %s len = %ld\n",order.parameters[i],strlen(order.parameters[i]));
        }
        //向服务端发送指令信号
        ssize_t retSend = send(sockfd,&order,sizeof(order),0);
        ERROR_CHECK(retSend, -1,"send order");
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //客户端选择具体业务执行
        switch(order.cmd){

        case LS:
            {
                char lsMsg[4096] = {0};
                int ret = recv(sockfd,lsMsg,4096,0);
                ERROR_CHECK(ret, -1, "recv lsMsg");
                printf("%s\n",lsMsg);
                break;
            }
        case CD:
            {
                //接收服务端发回来的返回值，判断之后的操作
                int ret = 0;
                recv(sockfd, &ret, sizeof(int), 0);
                if(ret == 0){
                    //改变当前命令提示符显示
                    changeCmdline(bufPrintf, &order);
                }
                break;
            }

        case PUTS:
            {
                
                int ret = commandPuts_C(order.parameters[0],sockfd);
                if(ret == -1){
                    printf("上传失败\n");
                }

                break;
            }
        case GETS:
            {
                int ret = client_download(sockfd, order.parameters[0]);
                if(ret == -1){
                    printf("下载失败\n");
                }
                break;
            }
        case RM:
            {
                int flag = 0;
                int retRecv = recv(sockfd, &flag, sizeof(int),0);
                ERROR_CHECK(retRecv, -1,"recv rmdir");
                //判断是否删除成功
                if(flag == -1){//不成功就打印信息,成功不做任何操作
                    printf("删除失败\n");
                }


                break;
            }
        case PWD:
            {
                char vitualPath[1024] = {0};
                recv(sockfd,vitualPath,1024,MSG_WAITALL);
                printf("path = %s\n",vitualPath);
                break;
            }

        case MKDIR:

            {
                int flag = 0;
                int retRecv = recv(sockfd, &flag, sizeof(int),0);
                ERROR_CHECK(retRecv, -1,"recv rmdir");
                //判断是否删除成功
                if(flag == -1){//不成功就打印信息,成功不做任何操作
                    printf("创建目录失败\n");
                }

                break;
            }
        case RMDIR:
            {
                int flag = 0;
                int retRecv = recv(sockfd, &flag, sizeof(int),0);
                ERROR_CHECK(retRecv, -1,"recv rmdir");
                //判断是否删除成功
                if(flag == -1){//不成功就打印信息,成功不做任何操作
                    printf("删除目录失败\n");
                }
                break;
            }

        case QUIT:
            break;

        default:
            break;

        }
        //退出逻辑
        if(order.cmd == QUIT){
            printf("byebye\n");
            break;
        }
    }
    return 0;
}



////////////////////////
//命令行提示符函数
int displayCmdLine(char* bufPrintf){

    printf("%s",bufPrintf);
    fflush(stdout);
}
///////////////////////
//改变当前打印的命令提示符信息
int changeCmdline(char* bufPrintf, order_t* porder){

    if(memcmp(porder->parameters[0],"..",2)==0&&strlen(porder->parameters[0])==2){
        //cd ..
        int lastIndex = strlen(bufPrintf)-1;
        int i;
        for(i = lastIndex; i > 0; --i){
            if(bufPrintf[i] == '/'){
                memset(bufPrintf+i,0,strlen(bufPrintf)-i);
                break;
            }
        }
        bufPrintf[i] = '$';
        bufPrintf[i+1] = ' ';
    }
    else if(memcmp(porder->parameters[0],".",1)==0&&strlen(porder->parameters[0])==1);
    else{
        //cd dir1
        printf("%ld\n",strlen(bufPrintf));
        bufPrintf[strlen(bufPrintf)-2] = '/';
        bufPrintf[strlen(bufPrintf)-1] = 0;
        sprintf(bufPrintf, "%s%s%s",bufPrintf,porder->parameters[0],"$ ");
    }
    return 0;
}




