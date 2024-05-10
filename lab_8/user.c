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

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            srand(time(NULL) ^ (getpid()<<16));
            while (!end) {
                char message[10];
                for (int j = 0; j < 10; j++) {
                    message[j] = 'a' + rand() % 26;
                }

                strncpy(queue->messages_to_print[queue->tail], message, 10);
                sem_wait(shared_memory);
                queue->tail = (queue->tail + 1) % QUEUE_SIZE;
                sem_post(shared_memory);

                sem_post(messages_in_queue);
                printf("User %d has generated: %s and now waiting to add for queue \n", i, message);
                sem_wait(queue_sem);

                int sleep_time = rand() % 5 + 10;
                printf("User %d is going to sleep for %d seconds \n", i, sleep_time);
                sleep(sleep_time);
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