#ifndef __PWD_H__
#define __PWD_H__

#include "user_dir_stack.h"
#include "sql.h"


int pwd(MYSQL *conn, dirStackType *dirStk, char *str);


#endif