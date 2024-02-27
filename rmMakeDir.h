#ifndef _RM_MAKE_DIR_H
#define _RM_MAKE_DIR_H
#include <mysql/mysql.h>
#include<func.h>
int rm(const char *Path,char *username);
int makeDir(const char *Path,char *username,char *dirName);
#endif
