#include<stdio.h>
#include"ls_cd_lib.h"

int main(void)
{
    char command[STR_LEN] = {0, };
    char path[PATH_LEN];
    char *token[20];
    int byteRead;
    int token_num;
    int cd_result;
    int i;


    start_menu();

    while(1)
    {
        write(1 , "Input the command : " , 20);
        byteRead = read(0 , command , STR_LEN);

        command[byteRead] = 0;
        token_num = get_token(token, command);
        token[token_num] = NULL;

        if((strcmp("q", token[0]) == 0) || (strcmp("Q", token[0]) == 0))
        {
            printf("end program..\n");
            return 0;
        }
        else if(strcmp("cd", token[0]) == 0)
        {
            if(token_num > 1){
                cd_result = chdir(token[1]);
                if(cd_result == -1)
                    printf("cd error\n");
            }
            getcwd(path, PATH_LEN);
            printf("current path : %s\n", path);
        }
        else if(strcmp("ls", token[0]) == 0)
        {
            if(token_num > 1)
            {
                if(strcmp("-l", token[1]) == 0)
                {
                    do_ls(".");
                }
               if((strcmp("-R", token[1]) == 0 ) || (strcmp("-r", token[1]) == 0))
                {
                    r_directory(".");
                }
            }
           else
                do_simple_ls(".");

        }
        else
        {
            printf("not define.\n");
        }
        for(i=0; i<token_num; i++)
            free(token[i]);
    }

    return 0;

}
