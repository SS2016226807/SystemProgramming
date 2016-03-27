
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include"ls_cd_lib.h"

//---------------ls -l--------------------------------------

void do_ls(char dir_name[])
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if((dir_ptr = opendir(dir_name)) == NULL)
            fprintf(stderr, "ls or ls -l : cannot open %s \n", dir_name);
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            if(strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) ;
            else
                do_stat(direntp->d_name);
        }
        closedir(dir_ptr);
    }
}
//---------ls -------------------------------------------
void do_simple_ls(char dir_name[])
{
    DIR *dir_ptr;
    struct dirent *direntp;
    int cnt = 0;

    if((dir_ptr = opendir(dir_name)) == NULL)
            fprintf(stderr, "ls or ls -l : cannot open %s \n", dir_name);
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
        {
           if(strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) ;
           else
           {
                printf("%-15s ", direntp->d_name);
                cnt++;
                if(cnt > 2)
                {
                    printf("\n");
                    cnt = 0;
                }
           }
        }
        printf("\n");

        closedir(dir_ptr);
    }
}
//-------------ls -R------------------------------------------
void do_ls_r(char dir_name[])
{
    DIR *dir_ptr;
    struct dirent *dirtp;
    char path[STR_LEN] = {0, };

    if((dir_ptr = opendir(dir_name) )== NULL)
    {
      fprintf(stderr, "ls : cannot open %s \n", dir_name);
    }

    printf("current directory : %s\n", dir_name);

    while((dirtp = readdir(dir_ptr)) != NULL)
    {
        if(strcmp(dirtp->d_name, ".") == 0 || strcmp(dirtp->d_name, "..") == 0)   
            continue;

        if(dirtp->d_type == DT_DIR)
        {
            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, dirtp->d_name);
            do_ls_r(path);
        }
        else
        {
            printf("%s \t", dirtp->d_name);
        }

    }
    printf("\n");

    closedir(dir_ptr);
}

void do_stat(char *name)
{
    struct stat info;
    if(stat(name, &info) == -1)
        perror(name);
    else
        show_file_info(name, &info);
}

void show_file_info(char *name, struct stat *buf)
{
    char modestr[11];

    mode_to_letters(buf->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d \t", (int)buf->st_nlink);
    printf("%-8s", uid_to_name(buf->st_uid));
    printf("%-8s", uid_to_name(buf->st_gid));
    printf("%8ld\t", (long)buf->st_size);
    printf("% .12s\t", 4+ctime(&buf->st_mtime));
    printf("%3s\n", name);
}
void mode_to_letters(int mode, char str[])
{
    strcpy(str, "--------");
    if(S_ISDIR(mode)) str[0] = 'd';
    if(S_ISCHR(mode)) str[0] = 'c';
    if(S_ISBLK(mode)) str[0] = 'b';

    if(mode & S_IRUSR) str[1] = 'r';
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';

    if(mode & S_IRGRP) str[4] = 'r';
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';

    if(mode & S_IROTH) str[7] = 'r';
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';
}
char* uid_to_name(uid_t uid)
{
    struct passwd *pw_ptr;
    static char numstr[20];

    if((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}
char* gid_to_name(gid_t gid)
{
    struct group *grp_ptr;
    static char numstr[20];

    if((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;

}
//----------------------------------------------------

void start_menu(char *path)
{

    getcwd(path, PATH_LEN);
    printf("==========START LS ===============\n");
    printf("============<option>=============\n\n");
    printf("[1] ls -l : show detail \n");
    printf("[2] ls -R : show sub dir\n");
    printf("[3] cd : same pwd\n");
    printf("[4] cd [path] : change dir\n");
    printf("================================\n");
}
//-----------token ftr------------------------------
int is_blank(char ch)
{
    if(ch == ' ' || ch == '\t' || ch == '\n')
        return 1;
    else
        return 0;
}

void make_string(char **str_tocopy, char *str_copy, int len)
{
    *str_tocopy = (char *)malloc(strlen(str_copy)+1);
    strcpy(*str_tocopy, str_copy);
}

int get_token(char **pbuf, char *msg)
{
    int index = 0;
    int i = 0, j = 0;
    int len;
    char buf[30];

    if(msg == NULL)
        return 0;

    len = strlen(msg);

    while(index < len)
    {
        while(msg[index] != 0 && !is_blank(msg[index]))
            buf[i++] = msg[index++];

        buf[i] = 0;
        make_string(&pbuf[j++], buf, i);
        i = 0;
        index++;
    }
    return j;
}
