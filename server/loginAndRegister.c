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
int SignIn_Deal(int netfd,MYSQL *conn) {
    File_Data_t fileData;
    memset(&fileData, 0, sizeof(fileData));
    char sendBuf[1004] = {0};
    char username[40] = {0};
    char password[40] = {0};
    int ret;

    while (1) {
        GET_FILEDATA;
        if (strcmp(fileData.dataBuf, "signN") == 0) {
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
        printf("%s\n", query);

        if (mysql_query(conn, query)) {
            printf("signIn ERROR:%s\n", mysql_error(conn));
            //mysql_close(conn);
            SEND_ERROR;
            continue;
        } else {
            res = mysql_use_result(conn);
            row = mysql_fetch_row(res);
            if (row) {
                printf("signIn ERROR:username exist!\n");
                SEND_ERROR;
                //mysql_close(conn);
                continue;
            } else {
                printf("signIn SUCCESS:username don't exist!\n");
                SEND_SUCCESS;
            }
        }
        //mysql_close(conn);
        // 得到一个salt
        // char salt[21] = {0};
        char salt[21] = {0};
        generateSalt(salt);
        
        // 收取密码
        GET_FILEDATA;
        strcpy(password, fileData.dataBuf);

        // char *shadow;
        // shadow = (char*) calloc(100, sizeof(char));
        // shadow = crypt(password, salt);
        char shadow[255] = {0};
        // shadow[0] = '\0';
        strcpy(shadow, crypt(password, salt));
        // printf("shadow = %s\n", shadow);
        // printf("signIn shadow:%s\n", shadow);

        // 插入数据库
        bzero(query, sizeof(query));
        sprintf(query, "insert into users (username, salt, encrypted_password, tomb) values ('%s', '%s', '%s', %d);", username, salt, shadow, 0);

        if (mysql_query(conn, query)) {
            printf("signIn ERROR:%s\n", mysql_error(conn));
            SEND_ERROR;
            //mysql_close(conn);
            continue;
        } else {
            printf("signIn success\n");
            SEND_SUCCESS;
            //mysql_close(conn);
        }
        break;
    }
    return 0;
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
        printf("%s\n", query);

        if (mysql_query(conn, query)) {
            printf("LogIn ERROR:%s\n", mysql_error(conn));
            //mysql_close(conn);
            SEND_ERROR;
            continue;
        } else {
            res = mysql_use_result(conn);
            row = mysql_fetch_row(res);
            if (row) {
                printf("LogIn SUCCESS:username exist!\n");
                printf("fileData.dataBuf = %s\n", fileData.dataBuf);
                SEND_SUCCESS;
                printf("fileData.dataBuf = %s\n", fileData.dataBuf);
                // 传回salt
                bzero(&fileData, sizeof(fileData));
                strcpy(fileData.dataBuf, row[2]);
                SEND_FILEDATA;

                // 传回shadow
                bzero(&fileData, sizeof(fileData));
                strcpy(fileData.dataBuf, row[3]);
                SEND_FILEDATA;
            } else {
                SEND_ERROR;
                //mysql_close(conn);
                continue;
            }
        }
        //mysql_close(conn);

        GET_FILEDATA;
        if (strcmp(fileData.dataBuf, "error") == 0) {
            printf("LogIn ERROR: Password error!\n");
            continue;
        }
        break;
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
//        ret = SignIn_Deal(netfd, conn);
//        if(ret == -1){
//            close(netfd);
//            continue;
//        }
//        ret = LogIn_Deal(netfd, username, conn);
//        if(ret == -1){
//            close(netfd);
//        }
//        //登陆成功
//        printf("hello world! username = %s\n", username);
//        close(netfd);
//    }
//    mysql_close(conn);
//    close(sockfd);
//    return 0;
//}
