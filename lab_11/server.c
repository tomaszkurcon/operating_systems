#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define ALIVE_INTERVAL 5
#define TIMEOUT 5

typedef struct {
    int socket;
    char id[32];
    int active;
    time_t last_response_time;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// funkcje obslugujace polecenia
void send_to_all(char *message, int exclude_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        //wysylamy do wszystkich klientow oprocz samego siebie
        if (clients[i].active && clients[i].socket != exclude_socket) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_to_one(char *message, char *target_id) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].id, target_id) == 0) {
            send(clients[i].socket, message, strlen(message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int socket) {
    char buffer[BUFFER_SIZE] = "Active clients:\n";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            //dodajemy kazdego klienta
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    send(socket, buffer, strlen(buffer), 0);
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];
    char client_id[32];
    //odbieramy id klienta
    recv(client_socket, client_id, sizeof(client_id), 0);

    pthread_mutex_lock(&clients_mutex);
    //szukamy wolnego miejsca i przypisujemy tam dane o kliencie
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].socket = client_socket;
            strcpy(clients[i].id, client_id);
            clients[i].active = 1;
            clients[i].last_response_time = time(NULL);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        //odbieramy polecenia od klienta
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        if (strncmp(buffer, "LIST", 4) == 0) {
            list_clients(client_socket);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "[%s] %s\n", client_id, buffer + 5);
            send_to_all(message, client_socket);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            char target_id[32];
            sscanf(buffer + 5, "%s", target_id);
            char *message_start = strstr(buffer + 5, " ") + 1;
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "[%s] %s\n", client_id, message_start);
            send_to_one(message, target_id);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            printf("Client %s disconnected\n", client_id);
            break;
        } else if (strncmp(buffer, "ALIVE", 5) == 0) {
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].socket == client_socket) {
                    //aktualizujemy czas ostatniej odpowiedzi
                    clients[i].last_response_time = time(NULL);
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    //zamykamy polaczenie
    close(client_socket);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client_socket) {
            //oznaczamy klienta jako nieaktywnego
            clients[i].active = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

void *ping_clients(void *arg) {
    while (1) {
        sleep(ALIVE_INTERVAL);

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active) {
                // sprawdzamy czy klient odpowiedzial w ciagu TIMEOUT, jesli nie to go usuwamy
                if (time(NULL) - clients[i].last_response_time > TIMEOUT) {
                    printf("Client %s timed out\n", clients[i].id);
                    close(clients[i].socket);
                    clients[i].active = 0;
                } else {
                    // w przeciwnym wypadku wysylamy ping
                    printf("Pinging client %s\n", clients[i].id);
                    if (send(clients[i].socket, "ALIVE", 5, 0) <= 0) {
                        clients[i].active = 0;
                        printf("Client %s is not responding\n", clients[i].id);
                    }
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        exit(EXIT_FAILURE);
    }
    char *address = argv[1];
    int port = atoi(argv[2]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    // konwertujemy adres na binarna forme
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping_clients, NULL);

    printf("Server listening on %s:%d\n", address, port);

    while (1) {

        int client_socket = accept(server_socket,NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, &client_socket);

        // gdy watek sie zakonczy, automatycznie zwalniamy zasoby
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
