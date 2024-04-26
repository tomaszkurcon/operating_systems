#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handleError(int signum){
    printf("Obsluga sygnalu SIGUSR1\n");
}

int main(int argc, char* argv[]){

    char* arg = argv[1];
    if (strcmp(arg, "none") == 0)
    {

    }
    if (strcmp(arg, "ignore") == 0)
    {
        signal(SIGUSR1, SIG_IGN);
    }
    if (strcmp(arg, "handle") == 0)
    {
        signal(SIGUSR1, handleError);
    }
    if (strcmp(arg, "mask") == 0)
    {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        if (sigprocmask(SIG_SETMASK, &newmask, NULL) < 0) {
            perror("Nie udało się zablokować sygnału");
        }
    }

    raise(SIGUSR1);
    if(strcmp(arg, "mask") == 0) {
        sigset_t pending_signals;
        int signal_number;
        if (sigpending(&pending_signals) == -1) {
            perror("sigpending");
            return 1;
        }
        for (signal_number = 1; signal_number < NSIG; signal_number++) {
            if (sigismember(&pending_signals, signal_number)) {
                printf("Sygnał %d jest oczekujący.\n", signal_number);
            }
        }
    }
    return 0;
}