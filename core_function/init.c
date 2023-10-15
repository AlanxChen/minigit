#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../repository/repository.h"
#include "../repository/config_msg.h"

int cmd_init(int argc,char* argv[])
{
    if(argc<3)
    {
        char *path=".";
        struct repository rep;
        rep=repository_init(path);
        creat_config(rep.config);
        free_struct_rep(&rep);
        return 0;
    }    
    /*else
    {
        //实现options
        //纠错，错误处理
    }*/
    return 0;
}