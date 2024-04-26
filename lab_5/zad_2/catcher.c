#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
volatile int global_counter = 0;
volatile int mode = 0;
volatile int prev_mode = 0;
void handleError(int sig, siginfo_t *si, void *unused){
    printf("Obsluga sygnalu SIGUSR1\n");
    int new_mode = si->si_value.sival_int;
    printf("pid: %d, mode: %d \n", si->si_pid, si->si_value.sival_int);
    mode = new_mode;
    if(new_mode != prev_mode){
        global_counter+=1;
        prev_mode = new_mode;
    }
    kill(si->si_pid, SIGUSR1);
}

int main() {
    printf("Catcher PID: %d \n", getpid());
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handleError;
    sigaction(SIGUSR1, &sa, NULL);

    while(1) {
        switch(mode){
            case 1:
                for(int i = 1; i<=100; i++) {
                    printf("%d \n", i);
                }
                mode = 0;
                break;
            case 2:
                printf("%d \n", global_counter);
                mode = 0;
                break;
            case 3:
                exit(0);
        }

    }

}

