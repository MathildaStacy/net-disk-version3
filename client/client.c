#include "analyOrder.h"
#include "exePanClient.h"
#include "loginAndRegister.h"
int main(int argc, char *argv[])
{
    // 192.168.72.128:1234 是服务端的ip端口
    ARGS_CHECK(argc,3);
    //与服务端建立连接
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    ERROR_CHECK(ret,-1,"connect");
    
    //执行登录业务
    printf("--------NetDisk--------\n");
    char username[40] = {0}; 
    //注册
    userRegister(username,sockfd);
    //登陆
    userLogin(username, sockfd);
    printf("Login successfully, username = %s\n",username);



    
    //执行网盘业务
    exePanClient(sockfd, username);
    close(sockfd);
    return 0;
}






















