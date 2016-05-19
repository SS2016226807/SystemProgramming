#include <stdio.h>
#include <signal.h>

#define MAXARGS 20
#define ARGLEN 100

char *makestring(char *buf);
void execute(char *arglist[]);

int main()
{
    char *arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];

    numargs = 0;
    while(numargs < MAXARGS)
    {
        printf("Arg[%d]? " , numargs);
        if(fgets(argbuf, ARGLEN , stdin) && *argbuf != '\n')
            arglist[numargs++] = makestring(argbuf);
        else{
            if(numargs>0){
                arglist[numargs] = NULL ;
                execute(arglist);
                numargs =0 ;
            }
        }
    }

    return 0;
}

void execute(char *arglist[])
{
    int pid , exitstatus ;

    pid = fork();
    switch(pid)
    {
    case -1:
        perror("fork failed");
        exit(1);

    case 0:
        execvp(arglist[0] , arglist);
        perror("execvp failed");
        exit(1);

    default :
        signal(SIGINT , SIG_IGN);
        while(wait(&exitstatus) != pid);
        printf("\nchild exited with status %d,%d\n\n" , exitstatus>>8 , exitstatus&0377);
        signal(SIGINT , SIG_DFL);

    }
}

char *makestring(char *buf)
{
    char *cp ;
    buf[strlen(buf) - 1] = '\0';
    cp = malloc(strlen(buf) + 1);
    if(cp == NULL)
    {
        fprintf(stderr , "no memory\n");
        exit(1);
    }

    strcpy(cp, buf);
    return cp;
}
