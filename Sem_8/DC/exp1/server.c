#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5555
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    SOCKET socket;
    char nickname[50];
} client_t;

client_t *clients[MAX_CLIENTS];
int client_count = 0;

void broadcast(char *message, SOCKET sender_socket) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i]->socket != sender_socket) {
            send(clients[i]->socket, message, strlen(message), 0);
        }
    }
}

unsigned __stdcall handle_client(void *arg) {
    SOCKET client_socket = (SOCKET)arg;
    char buffer[BUFFER_SIZE];
    int recv_len;

    // Get nickname
    send(client_socket, "NICK", 4, 0);
    recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (recv_len <= 0) {
        closesocket(client_socket);
        return 1;
    }
    buffer[recv_len] = '\0';
    
    int current_client_index = -1;
    for(int i=0; i<client_count; ++i){
        if(clients[i]->socket == client_socket){
            strcpy(clients[i]->nickname, buffer);
            current_client_index = i;
            break;
        }
    }

    printf("Nickname of client is %s\n", buffer);
    char join_msg[128];
    sprintf(join_msg, "%s joined the chat!", buffer);
    broadcast(join_msg, client_socket);

    while ((recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[recv_len] = '\0';
        printf("[MESSAGE] %s\n", buffer);
        broadcast(buffer, client_socket);
    }

    closesocket(client_socket);
    if(current_client_index != -1){
        char leave_msg[128];
        sprintf(leave_msg, "%s left the chat.", clients[current_client_index]->nickname);
        broadcast(leave_msg, client_socket);
        free(clients[current_client_index]);
        for(int i = current_client_index; i < client_count - 1; i++){
            clients[i] = clients[i+1];
        }
        client_count--;
    }
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);

    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    listen(server_socket, 3);

    puts("Waiting for incoming connections...");

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) != INVALID_SOCKET) {
        puts("Connection accepted");

        if (client_count < MAX_CLIENTS) {
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->socket = client_socket;
            clients[client_count++] = new_client;
            _beginthreadex(NULL, 0, &handle_client, (void*)client_socket, 0, NULL);
        } else {
            char *message = "Server is full.";
            send(client_socket, message, strlen(message), 0);
            closesocket(client_socket);
        }
    }

    if (client_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
        return 1;
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
