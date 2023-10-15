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

struct commit_object parse_data(const char * data)
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
int main()
{
 const char *conetent="tree 29ff16c9c14e2652b22f8b78bb08a5a07930c147\n\
parent 206941306e8a8af65b66eaaaea388a7ae24d49a0\n\
author Thibault Polge <thibault@thb.lt> 1527025023 +0200\n\
committer Thibault Polge <thibault@thb.lt> 1527025044 +0200\n\
gpgsig -----BEGIN PGP SIGNATURE-----\n\
\n\
 iQIzBAABCAAdFiEExwXquOM8bWb4Q2zVGxM2FxoLkGQFAlsEjZQACgkQGxM2FxoL\n\
 kGQdcBAAqPP+ln4nGDd2gETXjvOpOxLzIMEw4A9gU6CzWzm+oB8mEIKyaH0UFIPh\n\
 rNUZ1j7/ZGFNeBDtT55LPdPIQw4KKlcf6kC8MPWP3qSu3xHqx12C5zyai2duFZUU\n\
 wqOt9iCFCscFQYqKs3xsHI+ncQb+PGjVZA8+jPw7nrPIkeSXQV2aZb1E68wa2YIL\n\
 3eYgTUKz34cB6tAq9YwHnZpyPx8UJCZGkshpJmgtZ3mCbtQaO17LoihnqPn4UOMr\n\
 V75R/7FjSuPLS8NaZF4wfi52btXMSxO/u7GuoJkzJscP3p4qtwe6Rl9dc1XC8P7k\n\
 NIbGZ5Yg5cEPcfmhgXFOhQZkD0yxcJqBUcoFpnp2vu5XJl2E5I/quIyVxUXi6O6c\n\
 /obspcvace4wy8uO0bdVhc4nJ+Rla4InVSJaUaBeiHTW8kReSFYyMmDCzLjGIu1q\n\
 doU61OM3Zv1ptsLu3gUE6GU27iWYj2RWN3e3HE4Sbd89IFwLXNdSuM0ifDLZk7AQ\n\
 WBhRhipCCgZhkj9g2NEk7jRVslti1NdN5zoQLaJNqSwO1MtxTmJ15Ksk3QP6kfLB\n\
 Q52UWybBzpaP9HEd4XnR+HuQ4k2K0ns2KgNImsNvIyFwbpMUyUWLMPimaV1DWUXo\n\
 5SBjDB/V/W2JBFR+XKHFJeFwYhj7DD/ocsGr4ZMx/lgc8rjIBkI=\n\
 =lgTX\n\
 -----END PGP SIGNATURE-----\n\
\n\
Create first draft\n ";

parse_data(conetent);
return 0;
}