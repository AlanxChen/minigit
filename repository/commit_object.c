#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct commit_pair
{
    char *value;
    char *key;
} commit_pair;

struct  commit_object
{
    commit_pair *obj_pair_dic;
    int num;
    int data_len;
};
struct commit_object parse_data(const char * data)//测试通过
{
    struct commit_object the_obj;
    the_obj.data_len=strlen(data);
    int base_num=5;
    the_obj.obj_pair_dic=(commit_pair *)malloc(sizeof(commit_pair)*base_num);
    if(the_obj.obj_pair_dic==NULL)
    {
        perror("内存分配失败\n");
        exit(1);
    }
    char* copy = strdup(data);
    if (copy == NULL) {
        perror( "Memory allocation failed.\n");
     exit(1);
    }

    char* line = copy;
    char* token;
    int count=0;
    const char * gp_str="gpgsig";
    while ((token = strsep(&line, "\n")) != NULL) {
       count+=1;
       if(count>=base_num)
           {
            base_num*=2;
            commit_pair *temp=realloc(the_obj.obj_pair_dic,base_num*sizeof(commit_pair));
            if(temp==NULL)
            {
               
            free(the_obj.obj_pair_dic);
            perror("内存分配失败\n");
            exit(1);
            }
            the_obj.obj_pair_dic=temp;
           }
        char* key = strtok(token, " ");
        the_obj.obj_pair_dic[count].key=(char *) malloc(sizeof(char)*strlen(key));
        if(strcmp(key,gp_str)==0)
        {
        char* value = strtok(NULL, "\n");
        the_obj.obj_pair_dic[count].value=(char *) malloc(sizeof(char)*(strlen(line)+strlen(value)+sizeof(char)*10));  
        if(the_obj.obj_pair_dic[count].value==NULL)
        {
            printf("内存分配失败\n");
            free(the_obj.obj_pair_dic);
            exit(1);
        }  
        int i=strlen(value);
        value[i-1]='\n';
        the_obj.obj_pair_dic[count].value=value;
        the_obj.obj_pair_dic[count].value=strcat(value,line);
        //printf("key:%s,Value:%s\n",the_obj.obj_pair_dic[count].key, the_obj.obj_pair_dic[count].value);//输出作用
         break;
        
        }
        char* value = strtok(NULL, "\n");
        the_obj.obj_pair_dic[count].value=(char *) malloc(sizeof(char)*strlen(value));     
        
        if (key != NULL && value != NULL) {
            the_obj.obj_pair_dic[count].key=key;
            the_obj.obj_pair_dic[count].value=value;
            printf("key:%s,Value:%s\n",the_obj.obj_pair_dic[count].key, the_obj.obj_pair_dic[count].value);
        }
    }

    free(copy);
    the_obj.num=count;
    return the_obj;
}
int serialize_data(struct  commit_object com_obj)//尚未测试
{   
    char * the_obj_msg;
    the_obj_msg=(char * )malloc(sizeof(char)*(com_obj.data_len+20));
    if (the_obj_msg == NULL)
    {
        // 内存分配失败的处理
        return 1;
    }    
    int len = com_obj.num;
    int len_rec = 0;
    for (int i = 0; i < len; i++)
    {
        strcpy(the_obj_msg + len_rec, com_obj.obj_pair_dic[i].key);
        len_rec += strlen(com_obj.obj_pair_dic[i].key);
        the_obj_msg[len_rec++] = ' ';
        strcpy(the_obj_msg + len_rec, com_obj.obj_pair_dic[i].value);
        len_rec += strlen(com_obj.obj_pair_dic[i].value);
        the_obj_msg[len_rec++] = '\n';
    }
    if (len_rec >= 2)
    {
        the_obj_msg[len_rec - 2] = '\0';
    }
    return the_obj_msg;
}
int log_generate(char *HEAD)//head->获得refs/heads中的commit的hash值->根据hash值获得commit->输出元数据->查找父对象,遍历树
{;}
// https://fancyerii.github.io/gittutorials/4/#%E6%8F%90%E4%BA%A4%E5%9B%BEcommit-graph
/*
git log实现的底层原理：git log命令的底层原理涉及到 Git 的版本控制数据结构和底层对象存储。

Git 使用了一种称为有向无环图（DAG）的数据结构来存储版本控制历史。这个图由提交对象（commit object）和分支引用（branch reference）组成。

每个提交对象都是一个包含元数据和指向父提交的指针的独立实体。提交对象的元数据包括作者、提交日期、提交消息等信息。父提交指针指向当前提交的直接父提交，通过这种方式形成了提交对象之间的有向关系。

分支引用是指向提交对象的可变指针，用于记录当前分支的最新提交。分支引用指向的提交对象随着新的提交而改变，从而使得分支引用所代表的分支在版本控制历史中前进。

当执行 git log 命令时，Git会从当前分支引用开始，按照提交对象之间的有向关系进行遍历。具体步骤如下：

获取当前分支引用的指向的最新提交对象（HEAD 指向的提交）。
输出该提交对象的元数据（作者、提交日期、提交消息等）。
如果该提交对象有父提交，继续遍历父提交并输出其元数据，重复此步骤直到到达根提交（没有父提交）。
回到上一个提交对象，继续遍历其父提交并输出元数据。
重复步骤 3 和步骤 4，直到遍历完整个版本控制历史链。
通过这种方式，git log 命令按照提交对象之间的有向关系从最新的提交开始，逐步回溯到较早的提交，以展示版本控制历史记录。

值得注意的是，git log 命令还提供了一些选项和参数，用于控制输出的格式、限制输出的提交数量、过滤特定分支的提交记录等。

总结来说，git log 命令的底层原理是通过遍历提交对象的有向关系，按照提交的时间顺序展示版本控制历史记录。这种遍历是基于 Git 的版本控制数据结构和底层对象存储的。

 git中的分支存储在refs/heads中


*/