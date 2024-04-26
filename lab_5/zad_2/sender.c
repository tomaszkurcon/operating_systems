#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char* argv[]) {
    int pid = atoi(argv[1]);
    int arg = atoi(argv[2]);

    union sigval value;
    value.sival_int = arg;
    printf("%d", value.sival_int);
    if(sigqueue(pid, SIGUSR1, value) == -1) {
        perror("Nie udało się wysłać sygnału \n ");
        return 1;
    }
    sigset_t signals;
    sigfillset(&signals);
    sigdelset(&signals, SIGUSR1);

    sigsuspend(&signals);


    return 0;
}