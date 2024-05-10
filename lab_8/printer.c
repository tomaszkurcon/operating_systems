#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include "specs.h"

volatile bool end = false;
struct print_queue {
    char messages_to_print[QUEUE_SIZE][10];
    int head;
    int tail;
};

void sigint_handler(int sig_num) {
    end = true;
}
int main() {
    signal(SIGINT, sigint_handler);
    sem_t *printer_sem = sem_open("/printer_sem", O_CREAT, 0666, M);
    sem_t *queue_sem = sem_open("/queue_sem", O_CREAT, 0666, QUEUE_SIZE);
    sem_t *messages_in_queue = sem_open("/messages_sem", O_CREAT, 0666, 0);
    sem_t *shared_memory = sem_open("/shared_memory", O_CREAT, 0666, 1);
    int fd = shm_open("/print_queue", O_CREAT | O_RDWR, 0666);

    ftruncate(fd, sizeof(struct print_queue));
    struct print_queue *queue = mmap(NULL, sizeof(struct print_queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            while (!end) {
                sem_wait(printer_sem);
                printf("There is available printer \n");
                sem_wait(messages_in_queue);
                printf("There is message in queue \n");
                char message[10];
                sem_wait(shared_memory);
                strncpy(message, queue->messages_to_print[queue->head], 10);
                sem_post(shared_memory);
                sem_post(queue_sem);
                queue->head = (queue->head + 1) % QUEUE_SIZE;
                for (int j = 0; j < 10; j++) {
                    if(end) {
                        break;
                    }
                    printf("Printer %d: %c\n", i, message[j]);
                    sleep(1);
                    if(j == 9) {
                        printf("Printer %d: Done printing: %s\n", i, message);
                    }
                }

                sem_post(printer_sem);

            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {}

    munmap(queue, sizeof(struct print_queue));
    close(fd);
    shm_unlink("/print_queue");

    sem_close(printer_sem);
    sem_unlink("/printer_sem");
    sem_close(queue_sem);
    sem_unlink("/queue_sem");

    return 0;
}