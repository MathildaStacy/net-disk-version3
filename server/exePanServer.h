#ifndef _EXEPANSERVER_
#define _EXEPANSERVER_
#include "analyOrder.h"
#include <mysql/mysql.h>


int exePanServer(int netfd, MYSQL* mysqlfd, char* usrname);
//1 第一个参数是与客户连接传输信息的netfd,在用户退出时，关闭
//2 第二个参数是与本机mysql之间一个MYSQL链接
//3 第三个参数是用户登录的用户名

#endif
