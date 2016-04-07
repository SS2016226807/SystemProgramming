#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

typedef struct path_info
{
    char cur_dir_name[BUFSIZ] ;
    struct path_info *next;
}pathinfo;

typedef struct list_st
{
    pathinfo *head;
    pathinfo *cur;
    pathinfo *before;
}List;

ino_t get_inode(char *);
void printpathto(ino_t, List *);
void inum_to_name(ino_t , char *, int );
void list_init(List *);
void list_insert(List *, char * );
int first(List *, char * );
int next(List *, char *);


//-------------list-----------------------
void list_init(List *plist)
{
    plist->head = (pathinfo*)malloc(sizeof(pathinfo));
    plist->head->next = NULL;
}
void list_insert(List *plist, char *path_info)
{
    pathinfo *newpathinfo = (pathinfo*)malloc(sizeof(pathinfo));
    strcpy(newpathinfo->cur_dir_name, path_info);
    newpathinfo->next = plist->head->next;
    plist->head->next = newpathinfo;
}
int first(List *plist, char *info_name)
{
    if(plist->head->next == NULL)
        return 0;
    plist->before = plist->head;
    plist->cur = plist->head->next;

    strcpy(info_name, plist->cur->cur_dir_name);
    return 1;
}
int next(List *plist, char *path_info)
{
    if(plist->cur->next == NULL)
        return 0;
    plist->before = plist->cur;
    plist->cur = plist->cur->next;

    strcpy(path_info, plist->cur->cur_dir_name);
    return 1;
}
//------------------------------------------------------------------
int main(void)
{
    List my_list;
    list_init(&my_list);
    char t_name[BUFSIZ];

    printpathto(get_inode("."), &my_list);
    if(first(&my_list, t_name))
    {
        printf("/%s", t_name);
        while(next(&my_list, t_name))
            printf("/%s", t_name);
    }

    putchar('\n');
    return 0;
}

ino_t get_inode(char *fname)
{
    struct stat info;

    if(stat(fname, &info) == -1)
    {
        fprintf(stderr, "cannot stat");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}

void printpathto(ino_t this_inode, List *plist)
{
    ino_t my_inode;
    char its_name[BUFSIZ];

    my_inode = this_inode;

    while(get_inode("..")  != my_inode)
    {
        chdir("..")   ;
        inum_to_name(my_inode, its_name, BUFSIZ);
        list_insert(plist, its_name);
        my_inode = get_inode(".");
    }
}

void inum_to_name(ino_t inode_to_fine, char *namebuf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(".");
    if(dir_ptr == NULL)
    {
        perror(".");
        exit(1);
    }

    while((direntp = readdir(dir_ptr)) != NULL )
    {
        if(direntp->d_ino == inode_to_fine)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }

    }
    fprintf(stderr, "error looking for inum %d\n", inode_to_fine);
    exit(1);
}
