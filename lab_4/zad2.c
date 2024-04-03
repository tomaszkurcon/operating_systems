#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 0;
int main (int argc, char *argv[])
{
    char* name = argv[0];
    printf("%s \n", name);
    int local = 0;

    pid_t child_pid = fork();
    if(child_pid == 0) {
        printf("child process \n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d \n", getpid(), getppid());
        printf("child's local = %d, child's global = %d \n", local,global);
//NULL na końcu oznacza koniec listy argumentów!
        int status = execl("/bin/ls", "ls", argv[1], NULL);
        exit(status);
    } else {
        int child_status;
        waitpid(child_pid, &child_status, 0);
        printf("parent process \n");
        printf("parent pid = %d, child pid = %d \n", getpid(), child_pid);
        if(WIFEXITED(child_status)) {
            int exit_status = WEXITSTATUS(child_status);
            printf("Child exit code: %d \n", exit_status);
        }
        printf("Parents's local = %d, parent's global = %d \n", local, global);
        exit(child_status);
    }

  }
