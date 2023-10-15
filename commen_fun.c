#include "commen_fun.h"
int create_file(const char * path)
{
    FILE *file;
    file = fopen(path, "w");
    if (file == NULL) {
        printf("无法创建文件\n");
        exit(1);
    }
    fclose(file);
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