#ifndef _DATABASE_STRUCT_H_
#define _DATABASE_STRUCT_H_
#include <stddef.h>

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

// 用户相关
int dbAddUser(const User* user);                       // 添加用户
User* dbFindUserByName(const char* username);          // 通过用户名查找用户
int dbDeleteUser(int userid);                          // 删除用户

// 文件相关
int dbAddFile(const File* file);                       // 添加文件记录
File* dbFindFileBySHA1(const char* sha1);              // 通过SHA1查找文件
File* dbFindFileByDirID(int directoryId, int userid); // 通过文件id从数据库查找目录下的文件
File** dbListFileByParentID(int preid, int userid); /*通过文件的父亲的id找到所有文件，ls命令将调用这个函数。
这个函数会返回一个File结构体指针的数组，以NULL空指针结尾。
*/

/**
 * 根据给定的普通文件名查询其SHA1值。
 * 
 * @param filename 普通文件名。
 * @param sha1Value 指向预先分配足够空间的字符串指针，用于存放查询到的SHA1值。
 * @param sha1ValueSize sha1Value字符串分配的大小。
 * @return 0 表示成功，非0表示失败代码。
 */
int dbFindSHA1ByFilename(const char* filename, char* sha1Value, size_t sha1ValueSize);
#endif