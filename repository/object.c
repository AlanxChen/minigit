#include<stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
#include "object.h"
#include "../commen_fun.h"
#define CHUNK_SIZE 16384
#define BUFFER_SIZE 4096
#define ADD_SPACE 70
void calculateHash(const char* inputString, unsigned char* hashValue) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();  // 选择哈希算法，这里使用 SHA-256

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, inputString, strlen(inputString));
    EVP_DigestFinal_ex(mdctx, hashValue, NULL);

    EVP_MD_CTX_free(mdctx);
}
int  object_to_hash(FILE *file,long file_size,unsigned char *object_hashValue,char **content)//目前实现的是生成blob类型的hash     unsigned char hashValue[SHA256_DIGEST_LENGTH];
{

    long new_file_size =file_size+ADD_SPACE;
    char buffer[new_file_size];
    size_t read_size= fread(buffer,1,file_size,file);
    buffer[read_size]='\0';
    strcat(*content,"blob ");
    char size_str[ADD_SPACE];
    sprintf(size_str,"%ld",file_size);//变成字符串  
    strcat(*content,size_str);
    strcat(*content,buffer);//此时content就是文件的字符串
    calculateHash(*content, object_hashValue);
    return 0;
}

int decompressFile(const char* inputFilePath, char** decompressedData) //二进制文件的解压缩
{
    FILE* inputFile = fopen(inputFilePath, "rb");
    if (inputFile == NULL) {
        printf("打开文件失败\n");
        return -1;
    }
    fseek(inputFile, 0, SEEK_END);
    long inputFileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    unsigned char* inputData = (unsigned char*)malloc(inputFileSize);
    if (inputData == NULL) {
        printf("申请内存失败\n");
        fclose(inputFile);
        return -1;
    }
    if (fread(inputData, 1, inputFileSize, inputFile) != inputFileSize) {
        printf("读取文件失败\n");
        fclose(inputFile);
        free(inputData);
        return -1;
    }
    fclose(inputFile);
    z_stream zlibStream;
    memset(&zlibStream, 0, sizeof(z_stream));
    zlibStream.avail_in = inputFileSize;
    zlibStream.next_in = inputData;
    if (inflateInit(&zlibStream) != Z_OK) {
        printf("初始化失败\n");
        free(inputData);
        return -1;
    }
    unsigned char outputBuffer[BUFFER_SIZE];
    unsigned char* decompressedBuffer = NULL;
    size_t decompressedBufferSize = 0;
    int status;
    do {
        zlibStream.avail_out = BUFFER_SIZE;
        zlibStream.next_out = outputBuffer;
        status = inflate(&zlibStream, Z_NO_FLUSH);
        if (status == Z_NEED_DICT || status == Z_DATA_ERROR || status == Z_MEM_ERROR) {
            printf("解压出错: %s\n", zlibStream.msg);
            inflateEnd(&zlibStream);
            free(inputData);
            free(decompressedBuffer);
            return -1;
        }
        size_t decompressedChunkSize = BUFFER_SIZE - zlibStream.avail_out;
        decompressedBuffer = (unsigned char*)realloc(decompressedBuffer, decompressedBufferSize + decompressedChunkSize);
        if (decompressedBuffer == NULL) {
            printf("申请解压内存失败.\n");
            inflateEnd(&zlibStream);
            free(inputData);
            return -1;
        }
        memcpy(decompressedBuffer + decompressedBufferSize, outputBuffer, decompressedChunkSize);
        decompressedBufferSize += decompressedChunkSize;
    } while (status != Z_STREAM_END);
    *decompressedData = (char*)decompressedBuffer;
    inflateEnd(&zlibStream);
    free(inputData);
    return decompressedBufferSize;
}
int compressFile(const char *content,const char *outputfile_path)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
     return -1;
    }
    long file_size=strlen(content);   
    char output[file_size];   
    strm.avail_in = file_size;
    strm.next_in = (Bytef *)content;
    strm.avail_out=file_size;
    strm.next_out=(Bytef *)output;
    FILE *out_file = fopen(outputfile_path, "ab");
    if (!out_file) {
        deflateEnd(&strm);
        return -1;
    }
    while(strm.avail_in>0)
    {
        if(deflate(&strm,Z_FINISH)==Z_STREAM_ERROR)
        {
            printf("压缩过程出错\n");
            exit(1);
        }
        size_t output_size=file_size-strm.avail_out;
        fwrite(output,1,output_size,out_file);
    }
    if(deflateEnd(&strm)!=Z_OK)
    {
        printf("压缩错误\n");
        return(1);
    }
    fclose(out_file);
    return 0;
}
/*int object_write(const char *path,char *hash_path)  //hash_path需要确定大小    //创建blob object的文件夹
{     //hash_path需要确定大小
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
    strcpy(hash_path,_object_path);
    free(_object_path);
    return 0;
}
*/
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
    FILE *file=fopen(path,"r");
    if (file == NULL) {
     printf("无法打开文件：%s\n", path);
    exit(1);
    }
    fseek(file,0,SEEK_END);
    long file_size=ftell(file);
    fseek(file,0,SEEK_SET);  
    unsigned char hashValue[SHA256_DIGEST_LENGTH];

    char *content=(char *)malloc(sizeof(char)*(file_size+ADD_SPACE));
    if(content==NULL)
    {
        printf("分配内存失败\n");
        exit(1);
    }
    object_to_hash(file,file_size,hashValue,&content);
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
    create_file(_object_path);
    //写入内容
    compressFile(content,_object_path);
    free(_object_path);
    free(content);
    fclose(file);
    return 0;
}
int object_read(unsigned char hashValue[SHA256_DIGEST_LENGTH],const char *type)
{
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
    strcat(_object_path,"/");
    for (int i=1;i<SHA256_DIGEST_LENGTH;i++)
    {
        sprintf(_object_path+strlen(_object_path),"%02x",hashValue[i]);        
    }
    _object_path[strlen(_object_path)]='\0';
    FILE *file=fopen(_object_path,"rb");
    if (file == NULL) {
    printf("无法打开文件：%s\n", _object_path);
    exit(1);
    }
    char *decompressedData;
    decompressFile(_object_path,&decompressedData);
    printf("内容是:%s",decompressedData);
    free(decompressedData);
    free(_object_path);
    return 0;
}