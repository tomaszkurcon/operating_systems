#include <stdio.h>
#include <mqueue.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


void receive_messages(char *queue_name, mqd_t mq_client) {

    while (1) {
        char buffer[100];
        memset(buffer, 0, sizeof(buffer));
        mq_receive(mq_client, buffer, 100, NULL);
        printf("%s \n", buffer);
    }
}


int main() {
    char init_message[25];
    char queue_name[20];
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;
    sprintf(queue_name,"/client_queue_%d", getpid());
    mqd_t mq_client;
    if((mq_client = mq_open(queue_name, O_CREAT | O_RDONLY, 0666, &attr)) == -1) {
        perror("mq_open");
        return 1;
    }
    mqd_t mq_server = mq_open("/server_queue", O_RDWR);

    sprintf(init_message,"INIT %s", queue_name);
    mq_send(mq_server, init_message, sizeof(init_message), 0);
    char id[20];
    char msg[20];
    mq_receive(mq_client, msg, 100, NULL);
    if(strncmp(msg, "error", 5) == 0) {
        printf("Too many clients\n");
        return 1;
    }
    strcpy(id, msg);
    if(fork()==0) {
        receive_messages(queue_name, mq_client);
    }else {
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            char message[100];
            sprintf(message, "%s %s", id, buffer);
            mq_send(mq_server, message, strlen(message), 0);
        }

    }

    return 0;
}