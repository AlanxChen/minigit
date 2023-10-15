#include "../repository/object.h"
void hex2bytes(const char *hex, unsigned char *bytes) {
    for(size_t i = 0; i < strlen(hex); i += 2) {
        sscanf(hex + i, "%2hhx", &bytes[i/2]);
    }
}
int cmd_cat_file (int argc,char* argv[])
{
    if(argc<3)
    {
return 0;
    }    
    else{
    char *type=argv[2];
    unsigned char hashValue[SHA256_DIGEST_LENGTH];
        if (strlen(argv[3]) != 2*SHA256_DIGEST_LENGTH) {
        printf("Error: Invalid SHA256 hash string length.\n");
        return -1;
    }
    hex2bytes(argv[3], hashValue);
    object_read( hashValue,type);
   return 0;
    }
}
/*
解析参数：cat-file 命令通常有一个参数用于指定对象的名称（SHA-1 值）。该参数可以是对象的完整哈希值，也可以是对象的部分哈希值。Git 首先解析该参数，确定要查找的对象。

访问对象存储：Git 的对象存储位于 .git/objects 目录下。对象的哈希值前两个字符作为子目录的名称，后面的字符作为文件名。cat-file 命令通过拼接子目录和文件名的方式，定位到要查找的对象文件。

读取对象：一旦找到对象文件，cat-file 命令会读取文件的内容。对象文件以压缩的形式存储，因此需要解压缩才能获取原始内容。

显示对象类型和内容：cat-file 命令根据对象文件的内容，确定对象的类型（blob、tree、commit 或 tag），然后将对象的类型和内容显示在控制台上。
*/