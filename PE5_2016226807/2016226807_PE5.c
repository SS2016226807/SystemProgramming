#include <stdio.h>
#include <signal.h>
#include <termios.h>

int get_response(char* question);
void set_crmode();
int tty_mode(int how);

int main(void)
{
    void f(int);
    int i ;
    signal(SIGINT , f);
    for(i=0 ; i<10 ; i++)
    {
        printf("hello\n");
        sleep(1);
    }

    return 0;
}

void f(int signum)
{
    int response;

    printf("interrupted! OK to quity(y/n)? ");

    switch(getchar())
    {
    case 'y':
    case 'Y':
        response = 0;
        break;
    case 'n':
    case 'N':
    case EOF:
        response = 1;
        break;
    }

    if(response == 0)
        exit(0);
}
