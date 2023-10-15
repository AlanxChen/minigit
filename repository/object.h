#ifndef OBJECT_H
#define OBJECT_H
#include<stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
struct object
{
    char *object_type;
    char *the_file_path;    //文件的地址
    size_t compressed_object_size;
    char *compressed_object_buffer;
    unsigned char hash_value[42];
};//需要包含对象类型,压缩数据,hash
/*static const char * object_type_str[]={
    "UNKNOWN",
    "COMMIT",
    "TREE",
    "BLOB",
};*/ //对象->有限个情况->判断是否在
int object_write(const char *path);
int object_read(unsigned char hashValue[SHA256_DIGEST_LENGTH],const char *type);
#endif