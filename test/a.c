#include<stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
#define ADD_SPACE 70
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
int main()
{
    unsigned char  p[20];
    const char *path="./test.txt";
    unsigned char hashValue[SHA256_DIGEST_LENGTH];
    object_to_hash(path,hashValue);
    p[0]='/'; 
    printf("The is %02x",hashValue[0]);
    return 0;
    
}