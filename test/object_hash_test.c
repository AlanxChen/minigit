#include<stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
#include "../commen_fun.h"
#define ADD_SPACE 70
struct object
{
    char *object_type;
    char *the_file_path;    //文件的地址
    size_t compressed_object_size;
    char *compressed_object_buffer;
    unsigned char hash_value[42];
};//需要包含对象类型,压缩数据,hash
static const char * object_type_str[]={
    "UNKNOWN",
    "COMMIT",
    "TREE",
    "BLOB",
};//对象->有限个情况->判断是否在
void calculateHash(const char* inputString, unsigned char* hashValue) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();  // 选择哈希算法，这里使用 SHA-256

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, inputString, strlen(inputString));
    EVP_DigestFinal_ex(mdctx, hashValue, NULL);

    EVP_MD_CTX_free(mdctx);
}
int  object_to_hash(const char *path,unsigned char *object_hashValue)//目前实现的是生成blob类型的hash     unsigned char hashValue[SHA256_DIGEST_LENGTH];
{
    FILE *file=fopen(path,"r");
    if (file == NULL) {
     printf("无法打开文件：%s\n", path);
    exit(1);
    }
    fseek(file,0,SEEK_END);
    long file_size=ftell(file);
    fseek(file,0,SEEK_SET);
    long new_file_size =file_size+ADD_SPACE;
    char buffer[new_file_size];
    size_t read_size= fread(buffer,1,file_size,file);
    buffer[read_size]='\0';
    char content[new_file_size];
    strcat(content,"blob ");
    char size_str[ADD_SPACE];
    sprintf(size_str,"%ld",file_size);//变成字符串  
    strcat(content,size_str);
    strcat(content,buffer);//此时content就是文件的字符串
    calculateHash(content, object_hashValue);
    return 0;
}
int create_folder(const char * path)
{
    if(mkdir(path,0777)!=0)
    {
        printf("创建文件%s失败\n",path);
        exit(1);
    }
    return 0;
}
void printHashValue(unsigned char *hashValue, int length) {
    int i;
    printf("文件的hash值是");
    for (i = 0; i < length; i++) {
        printf("%02x", hashValue[i]);
    }
    printf("\n");
}

int object_write(const char *path)
{
    unsigned char hashValue[SHA256_DIGEST_LENGTH];
    object_to_hash(path,hashValue);
    printHashValue(hashValue,SHA256_DIGEST_LENGTH);
    char *object_path="./.minigit/objects";
    char * _object_path=( char*)malloc(sizeof( char)*(ADD_SPACE+strlen(object_path)));
    if(_object_path==NULL)
    {
        printf("动态指针分配内存失败");
        exit(1);
    }
    strcpy(_object_path,object_path);
    strcat(_object_path,"/");
    for(int i=0;i<1;i++)
    {
        sprintf(_object_path+strlen(_object_path),"%02x",hashValue[i]);
    }
    _object_path[strlen(_object_path)]='\0';
    create_folder(_object_path);
    strcat(_object_path,"/");
    for (int i=1;i<SHA256_DIGEST_LENGTH;i++)
    {
        sprintf(_object_path+strlen(_object_path),"%02x",hashValue[i]);        
    }
        _object_path[strlen(_object_path)]='\0';
    create_folder(_object_path);
    free(_object_path);
    return 0;
}

int main() {
    const char *path="./test.txt";
    if(object_write(path)==0)
    {
    printf("创建成功\n");
    return 0;
    }
}
//gcc -o hash_program c.c -lssl -lcrypto
//gcc -o hash_program object_hash_test.c -lssl -lcrypto
'''
void printHashValue(unsigned char *hashValue, int length) {
    int i;

    for (i = 0; i < length; i++) {
        printf("%02x", hashValue[i]);
    }
    printf("\n");
}

int main() {
    char *path = "test";
    unsigned char hashValue[SHA256_DIGEST_LENGTH] = { /* hash value here */ };
    char combinedPath[SHA256_DIGEST_LENGTH * 2 + strlen(path) + 1];

    sprintf(combinedPath, "%s", path);
    printHashValue(hashValue, SHA256_DIGEST_LENGTH);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(combinedPath + strlen(combinedPath), "%02x", hashValue[i]);
    }

    printf("Combined Path: %s\n", combinedPath);

    return 0;
}
'''