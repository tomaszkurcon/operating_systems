#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int client_socket;
char client_id[32];

// handler dla sygnalu SIGINT
void handle_exit() {
    send(client_socket, "STOP", 4, 0);
    close(client_socket);
    exit(EXIT_SUCCESS);
}

// funkcja odbierajaca wiadomosci od serwera
void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);

        if (strncmp(buffer, "ALIVE", 5) == 0) {
            send(client_socket, "ALIVE", 5, 0);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        exit(EXIT_FAILURE);
    }
    strncpy(client_id, argv[1], sizeof(client_id) - 1);
    client_id[sizeof(client_id) - 1] = '\0';
    char *address = argv[2];
    int port = atoi(argv[3]);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // wyslanie ID klienta do serwera
    send(client_socket, client_id, strlen(client_id), 0);
    // obsluga sygnalu SIGINT
    signal(SIGINT, handle_exit);

    pthread_t recv_thread;
    // utworzenie watku do odbierania wiadomosci
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    char buffer[BUFFER_SIZE];
    while (1) {
        // mozliwosc wpisywania polecen
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "STOP") == 0) {
            handle_exit();
        }

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}
