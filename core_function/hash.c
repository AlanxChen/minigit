#include "../repository/object.h"
int cmd_hash_object (int argc,char* argv[])
{
    if(argc<3)
    {
        return 0;
    }   
    else
    {
        char * file_path=argv[2];
        object_write(file_path);
        return 0;
    }
}