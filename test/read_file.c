#include<stdio.h>
#include <string.h>
#define ADD_SPACE 30
int main()
{
    const char *path="./test.txt";
    FILE *file=fopen(path,"r");
    if (file == NULL) {
     fprintf(stderr, "无法打开文件：%s\n", path);
    return 1;
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
    strcat(content,buffer);
    printf("The str is %s",content);
    return 0;
}