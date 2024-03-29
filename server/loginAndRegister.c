#include "loginAndRegister.h"

#define BUF_SIZE 4096
#define PORT 9190
//生成盐值
void generateSalt(char *salt){
    salt[0] = '$';
    salt[1] = '6';
    salt[2] = '$';
    salt[19] = '$';
    salt[20] = 0;
    srand(time(NULL));
    int flag;
    for(int i = 3;i < 19;i++){
        flag = rand() % 3;
        switch(flag){
            case 0:
                salt[i] = rand() % 26 + 'a';
                break;
            case 1:
                salt[i] = rand() % 26 + 'A';
                break;
            case 2:
                salt[i] = rand() % 10 + '0';
                break;
        }
    }
}
//注册处理
int SignIn_Deal(int netfd, MYSQL *conn) {
    File_Data_t fileData;
    bzero(&fileData, sizeof(fileData));
    char sendBuf[1004] = {0};
    char username[40] = {0};
    char password[40] = {0};
    int ret;

    while (1) {
        GET_FILEDATA;
        if (strcmp(fileData.dataBuf, "signN") == 0) {
            printf("signin over\n");
            return 0;
        }

        // 得到用户名
        bzero(username, sizeof(username));
        GET_FILEDATA;
        strncpy(username, fileData.dataBuf, strlen(fileData.dataBuf) - 1);

        // 查询用户名是否存在
        MYSQL_RES *res;
        MYSQL_ROW row;
        char query[2000] = {0};
        sprintf(query, "select * from users where username = '%s';", username);

        if (mysql_query(conn, query)) {
            printf("signIn error:%s\n", mysql_error(conn));
            SEND_ERROR;
            mysql_free_result(res);
            continue;
        } else {
            res = mysql_use_result(conn);
            row = mysql_fetch_row(res);
            if (row) {
                printf("signIn error:username exist!\n");
                SEND_ERROR;
                mysql_free_result(res);
                continue;
            } else {
                printf("signIn success:username don't exist!\n");
                SEND_SUCCESS;
                mysql_free_result(res);
            }
        }

        char salt[21] = {0};
        generateSalt(salt);
        
        // 收取密码
        GET_FILEDATA;
        strcpy(password, fileData.dataBuf);

        char shadow[255] = {0};
        strcpy(shadow, crypt(password, salt));      

        // 插入数据库
        bzero(query, sizeof(query));
        sprintf(query, "insert into users (username, salt, encrypted_password, tomb) values ('%s', '%s', '%s', %d);", username, salt, shadow, 0);

        if (mysql_query(conn, query)) {
            printf("signIn error:%s\n", mysql_error(conn));
            SEND_ERROR;
            continue;
        } else {
            printf("signIn success\n");
            SEND_SUCCESS;
            
        }
        break;
    }
    return 1;
}

//登陆处理
int LogIn_Deal(int netfd, char *username, MYSQL *conn) {
    File_Data_t fileData;
    memset(&fileData, 0, sizeof(fileData));
    char sendBuf[1004] = {0};
    int ret;

    while (1) {
        // 接收用户名
        GET_FILEDATA;
        bzero(username, 40);
        strncpy(username, fileData.dataBuf, 40);

        // 查询用户名是否存在
        MYSQL_RES *res;
        MYSQL_ROW row;
        char query[2000] = {0};
        bzero(query, sizeof(query));
        
        sprintf(query, "select * from users where username = '%s';", username);
        if (mysql_query(conn, query)) {
            printf("LogIn error:%s\n", mysql_error(conn));
            SEND_ERROR;
            mysql_free_result(res);
            continue;
        } else {
            res = mysql_use_result(conn);
            row = mysql_fetch_row(res);
            if (row) {
                
                printf("LogIn success:username exist!\n");

                SEND_SUCCESS;
                //传回salt
                bzero(&fileData, sizeof(fileData));
                strcpy(fileData.dataBuf, row[2]);
                SEND_FILEDATA;

                //传回shadow
                bzero(&fileData, sizeof(fileData));
                strcpy(fileData.dataBuf, row[3]);
                SEND_FILEDATA;
                mysql_free_result(res);
            } else {
                SEND_ERROR;
                mysql_free_result(res);
                continue;
            }
        }

        GET_FILEDATA;
        if (strcmp(fileData.dataBuf, "error") == 0) {
            printf("LogIn error: Password error!\n");
            continue;
        }
        break;
    }
    return 0;
}

int NetDiskInterface(int netfd, MYSQL *conn, char *username){
    int ret = SignIn_Deal(netfd, conn);
    while(ret == 1){
        ret = SignIn_Deal(netfd, conn);
    }
    ret = LogIn_Deal(netfd, username, conn);
    if(ret == -1){
        close(netfd);
    }
    return 0;
}

//int main(int argc, char *argv[]) {
//    struct sockaddr_in serv_addr;
//    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
//    if(sockfd == -1){
//        perror("socket");
//        return -1;
//    }
//
//    memset(&serv_addr, 0 , sizeof(serv_addr));
//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    serv_addr.sin_port = htons(PORT);
//
//    int reuse = 1;
//    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
//    if(ret == -1){
//        perror("setsockopt");
//        return -1;
//    }
//
//    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//    listen(sockfd, 50);
//
//    MYSQL *conn = mysql_init(NULL);
//    if (conn == NULL) {
//        fprintf(stderr, "mysql_init() failed\n");
//        return -1;
//    }
//
//    if (mysql_real_connect(conn, "localhost", "root", "010123", "netdisk", 0, NULL, 0) == NULL) {
//        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
//        mysql_close(conn);
//        return -1;
//    }
//    printf("Mysql connect successfully!\n");
//
//    while(1){
//        int netfd = accept(sockfd, NULL, NULL);
//
//        char username[40] = {0};
//        NetDiskInterface(netfd, conn, username);
//        //登陆成功
//        printf("Login success, username = %s\n", username);
//        close(netfd);
//    }
//    mysql_close(conn);
//    close(sockfd);
//    return 0;
//}

