#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
    int arg = atoi(argv[1]);
    for(int i = 0; i <  arg; i++) {
        pid_t child_pid = fork();
        if(child_pid==0) {
            printf("Pid procesu własnego:%d Pid procesu rodzica ma pid:%d \n", (int)getpid(), (int)getppid());
            exit(0);
        }
    }
    while(wait(NULL) > 0);

    printf("%d \n", arg);

    return 0;
}
