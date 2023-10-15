#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PARMENT_NUM 40
typedef struct {
    char *key;
    char *value;
} config_element;

typedef struct {
    config_element *obj;
    int num;
} config_dic;
int creat_config(const char * filename)
{
    FILE *file=fopen(filename,"w");
    if(!file)
    {
        printf("打开文件失败 %s\n",filename);
        return 1;
    }
    char line[]="[core]\n\
    repositoryformatversion = 0\n\
    filemode = true\n\
    bare = false\n\
\n\
[user]\n\
    name = Your \n\
    email = your.email@example.com\n\
\n\
[alias]\n\
    co = checkout\n\
    ci = commit\n\
    st = status\n\
    br = branch\n";
    fputs(line,file);
    fclose(file);
    return 0;
}
int removeSpaces(char *str)
{
    int i, j;
    for (i = j = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}
config_dic * parse_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("打开文件失败: %s\n", filename);
        return NULL;
    }

    config_dic *dic = malloc(sizeof(config_dic));
    dic->num = 0;
    dic->obj = (config_element *)malloc(sizeof(config_element)*MAX_PARMENT_NUM);
    char line[256];
    while (fgets(line, sizeof(line), file)) {      
        if (line[0] == '[') {
            // 头部
        } else if (strchr(line, '=')!=NULL) {
            // 键值
            dic->obj[dic->num].key=strdup(strtok(line,"="));
            dic->obj[dic->num].value=strdup(strtok(NULL,"="));
            removeSpaces(dic->obj[dic->num].key);
            removeSpaces(dic->obj[dic->num].value);
            printf("The Parament'key is%s,value is %s", dic->obj[dic->num].key,dic->obj[dic->num].value);//测试使用
            dic->num++;
        }
    }

    return dic;
}
int free_config_dic(config_dic *dic)
{
    if(dic==NULL)
    {
        return 0;
    }
    if(dic->obj!=NULL)
    {
        for(int i=0;i<dic->num;i++)
        {
            free(dic->obj[i].key);
            free(dic->obj[i].value);
        }
        free(dic->obj);
    }
    free(dic);
    return 0;
}

int main()
{
    const char *path="./test.txt";
    if(creat_config(path)==0)
    {
       config_dic* dic= parse_config(path);
        if(free_config_dic(dic)==0) 
        {
            printf("释放成功\n");
            return 0;
        }
    }
    return 0;
}