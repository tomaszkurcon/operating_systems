#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define REINDEER_COUNT 9
#define DELIVERY_COUNT 4

int number_of_reindeers = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *santa_claus_thread(void *arg) {
    int delivers = 0;
    while(delivers < DELIVERY_COUNT) {
        pthread_mutex_lock(&mutex);
        while(number_of_reindeers < 9) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Mikołaj: budzę się \n");
        int deliver_time = rand()%3+2;
        printf("Mikołaj: dostarczam zabawki przez %d sekund\n", deliver_time);
        sleep(deliver_time);
        number_of_reindeers=0;
        delivers++;
        printf("Mikołaj: zasypiam\n");
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
    }
    return NULL;
}

void *reindeer_thread(void *arg){
    int id = *(int *)arg;
    while(1) {
        sleep(rand()%6+5);
        pthread_mutex_lock(&mutex);
        number_of_reindeers++;
        printf("Renifer %d: czeka %d reniferów na Mikołaja \n",id,number_of_reindeers);
        if(number_of_reindeers == REINDEER_COUNT) {
            printf("Renifer %d: budzę Mikołaja\n", id);
        }
        pthread_cond_broadcast(&cond);
        while(number_of_reindeers!=0) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(){
    pthread_t santa_thread;
    pthread_t reindeer_threads[REINDEER_COUNT];
    int ids[REINDEER_COUNT];
    srand(time(NULL));
    pthread_create(&santa_thread, NULL, santa_claus_thread, NULL);
    for(int i = 0; i < REINDEER_COUNT; i++){
        ids[i] = i+1;
        pthread_create(&reindeer_threads[i], NULL, reindeer_thread, &ids[i]);
    }
    pthread_join(santa_thread, NULL);
    for(int i = 0; i < REINDEER_COUNT; i++){
        pthread_cancel(reindeer_threads[i]);
    }
    return 0;
}