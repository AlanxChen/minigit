#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "repository.h"
#include "../commen_fun.h"

char * combine_str(const char *path,const char* bc_path)
{
    
    char *p=(char *)malloc(sizeof(char)*(strlen(path)+strlen(bc_path)+10));
    if(p==NULL)
    {
        printf("指针分配空间失败\n");
        exit(1);
    }
    strcat(p,path);
    strcat(p,bc_path);
    return p;
}

int create_rep(struct repository *rep)
{
    create_folder(rep->gitdir);
    create_file(rep->HEAD);
    create_file(rep->config);
    create_folder(rep->object);
    create_folder(rep->refs);
    return 0;

}
struct repository repository_init(char * path)
  {
    struct repository rep;
    rep.worktree=path;
    //char *_path=path;
    rep.gitdir=combine_str(path,"/.minigit");
    rep.config=combine_str(path,"/.minigit/config");
    rep.object=combine_str(path,"/.minigit/objects");
    rep.refs=combine_str(path,"/.minigit/refs");
    rep.HEAD=combine_str(path,"/.minigit/HEAD");
    create_rep(&rep);
    return rep;
  }
void free_struct_rep (struct repository *ptr)
  {
    if(ptr!=NULL)
    {
        free(ptr->config);
        free(ptr->gitdir);
        free(ptr->HEAD);
        free(ptr->object);
        free(ptr->refs);
    }
  }
/*int cmd_init(int argc,char*argv[])
  {
    if(argc<3)
    {
        char *path=".";
        struct repository rep;
        rep=repository_init(path);
        free_struct_rep(&rep);
        return 0;
    }
    else
    {
        //实现options
        //纠错，错误处理
    }
  }
  */