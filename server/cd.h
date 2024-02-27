#ifndef __CD_H__
#define __CD_H__

#include "user_dir_stack.h"
#include "sql.h"


int cd(MYSQL *conn, dirStackType *dirStk, char *str);

#endif