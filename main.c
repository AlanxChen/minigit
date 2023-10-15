#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "core_function.h"
struct cmd_struct{
    char * cmd_name;
    int (*cmd_func)();
};
static struct cmd_struct commands[]={
    {"init",cmd_init},
    {"hash-object",cmd_hash_object},
    {"cat-file",cmd_cat_file}
};
int main(int argc,char*argv[])
{
     char *cmd;
    cmd=argv[1];
    if(!cmd)
    {
        cmd="minigit-help";
    }

   for(int i=0;i<sizeof(commands)/sizeof(commands[0]);i++)
   {    
        if (strcmp(commands[i].cmd_name,cmd)==0)
        {
            commands[i].cmd_func(argc,argv);
            return 0;
        }
   }
   printf("%s命令不存在",cmd);
   return 0;


}
