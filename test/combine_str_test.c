#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#define ADD_SPACE 50
char * combine_str(const char *path,const char* bc_path)
{
    const int len=strlen(path)+strlen(bc_path)+10;
    char *p=(char *)malloc(sizeof(char)*len);
    if(p==NULL)
    {
        printf("指针分配空间失败\n");
        exit(1);
    }
    strcat(p,path);
    strcat(p,bc_path);
    char _p[len];
    strcpy(_p,p);
    free(p);
    return _p;
}

int main()
{
    char *path=".";
    char *new_path=combine_str(path,"/.minigit");
    if(mkdir(new_path,0777)!=0)
    {
        printf("创建文件%s失败\n",new_path);
        exit(1);
    }
    return 0;
}