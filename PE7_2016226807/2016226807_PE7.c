#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    int f_pipe[2];
    int s_pipe[2];

    int pid1, pid2;

    if(argc != 4)
    {
        printf("argv 4 : ./main cmd1 cmd2 cmd3\n");
        exit(1);
    }
    printf("\n\n");

    if(pipe(f_pipe) == -1){
        perror("pipe create error!");
        exit(1);
    }

    pid1 = fork();
    if(pid1 == -1){
        perror("first fork() error!");
        exit(1);
    }
    if(pid1 == 0)
    {
        close(f_pipe[1]);
        if(dup2(f_pipe[0], 0) == -1){
            perror("dup2() error!");
            exit(1);
        }
        close(f_pipe[0]);

        if(pipe(s_pipe) == -1){
            perror("pipe create error!");
            exit(1);
        }
        pid2 = fork();
        if(pid2 == 0)
        {
               close(s_pipe[1]);
               if(dup2(s_pipe[0], 0) == -1){
                   perror("dup2() error!");
                   exit(1);
               }
               close(s_pipe[0]);
	       printf("\n");
               execlp(argv[3], argv[3], NULL);

        }
        else
        {
            close(s_pipe[0]);
            if(dup2(s_pipe[1], 1) == -1){
                perror("dup2() error!");
                exit(1);
            }
            close(s_pipe[1]);
            execlp(argv[2], argv[2], NULL);
        }
    }
    else
    {
        close(f_pipe[0]);
        if( dup2(f_pipe[1], 1) == -1){
            perror("dup2() error!");
            exit(1);
        }
        close(f_pipe[1]);
        execlp(argv[1], argv[1], NULL);
    }

    return 0;
}
