#include <stdio.h>
#include <mqueue.h>
#include <string.h>

#define MAX_CLIENTS 2

struct client_info {
    mqd_t queue;
    char queue_name[20];
};

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_CLIENTS;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;

    mqd_t mqdes;
    if((mqdes = mq_open("/server_queue", O_CREAT | O_RDWR, 0666, &attr)) == -1) {
        perror("mq_open");
        return 1;
    }
    struct client_info clients[MAX_CLIENTS];
    int client_count = 0;

    while(1) {
        char message[100];
        if(mq_receive(mqdes, message, sizeof(message), NULL)==-1) {
            perror("mq_receivee");
            return 1;
        };
        if (strncmp(message, "INIT", 4) == 0) {
            if (client_count == MAX_CLIENTS) {
                printf("Too many clients\n");
                mqd_t client_queue = mq_open(message + 5, O_RDWR);
                char message_to_send[100] = "error";
                mq_send(client_queue, message_to_send, strlen(message_to_send), 0);
                continue;
            }
            strcpy(clients[client_count].queue_name, message + 5);
            clients[client_count].queue = mq_open(clients[client_count].queue_name, O_RDWR);
            char id_message[20];
            sprintf(id_message, "%d", client_count);
            mq_send(clients[client_count].queue, id_message, strlen(id_message), 0);
            client_count++;
        } else {
            int id;
            char message_to_send[100];

            sscanf(message, "%d %[^\n]", &id, message_to_send);
            printf("%d - id \n", id);
            printf("%s - message to send \n", message_to_send);
            for (int i = 0; i < client_count; i++) {
                if (i == id) {
                    continue;
                }
                mq_send(clients[i].queue, message_to_send, strlen(message_to_send), 0);
            }
        }
    }

    return 0;
}