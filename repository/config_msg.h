#ifndef CONFIG_MSG_H
#define CONFIG_MSG_H
typedef struct {
    char *key;
    char *value;
} config_element;
typedef struct {
    config_element *obj;
    int num;
} config_dic;
int creat_config(const char * filename); //实现在使用git init的时候对config文件进行初始化
config_dic * parse_config(const char *filename); //对config 文件进行解析，获取参数，可实现修改config参数和添加参数的功能
int free_config_dic(config_dic *dic); //释放parse_config()函数产生的动态指针


#endif