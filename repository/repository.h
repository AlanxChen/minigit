#ifndef REPOSITORY_H
#define REPOSITORY_H
struct repository{
  char *worktree;
  char * gitdir;
  char *config;
  char * object;
  char * refs;
  char *HEAD;
  };
struct repository repository_init(char * path); 
void free_struct_rep (struct repository *ptr);
#endif