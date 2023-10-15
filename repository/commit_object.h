#ifndef COMIMIT_OBJECT_H
#define COMMIT_OBJECT_H
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
struct commit_object parse_data(const char * data);
int serialize_data(struct  commit_object com_obj);
#endif