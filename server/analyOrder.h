#ifndef _ANALYRODER_
#define _ANALYRODER_

enum{

    CD,
    LS,
    PUTS,
    GETS,
    RM,
    PWD,
    MKDIR,
    RMDIR,
    QUIT
};
typedef struct order_s{
    
    int cmd;//命令
    char parameters[10][32];
    int num; //参数的最大数量
}order_t;

//解析指令
//1、功能 拆分用户输入，判断是否合法，并将命令按格式输入到结构体中
int analyOrder(char* order, order_t* porder);
//解析命令
int analyCmd(char* cmd, order_t* porder);
//初始化结构体
int orderInit(order_t* porder);//参数数量
#endif
