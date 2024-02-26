#ifndef _DATABASE_STRUCT_H_
#define _DATABASE_STRUCT_H_

typedef struct {
    int userid;         // 用户id
    char* username;     // 用户名
    char* salt;         // 盐值
    char* password;     // 密码
    int tomb;           // 墓碑值
} User;


typedef struct {
    int fileId;         // 文件id
    char* filename;     // 文件名
    int user;           // 文件所属用户id
    int pre_id;         // 上一级目录id
    // 从根目录开始的绝对路径：根据你的需要选择是否实现
    char* absPath;      // 绝对路径（可选）
    char* type;         // 文件类型
    char* sha1;         // 文件的sha1值
    int tomb;           // 墓碑值
} File;

#endif