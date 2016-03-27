#ifndef LSLIB_H
#define LSLIB_H

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

#define STR_LEN 255
#define PATH_LEN 255
#define TOKEN_SIZE 30
// ls ftr
void do_ls(char dir_name[]);
void do_stat(char *name);
void show_file_info(char *name, struct stat *buf);
void mode_to_letters(int mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
void start_menu(char *path);
void do_ls_r(char dir_name[]);
void do_simple_ls(char dir_name[]);
//string token ftr
int is_blank(char ch);
void make_string(char **str_tocopy, char *str_copy, int len);
int get_token(char **pbuf, char *msg);

#endif // LSLIB_H

