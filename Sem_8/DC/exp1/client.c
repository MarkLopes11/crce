#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 5555
#define BUFFER_SIZE 1024

unsigned __stdcall receive_messages(void *arg) {
    SOCKET client_socket = (SOCKET)arg;
    char buffer[BUFFER_SIZE];
    int recv_len;

    while ((recv_len = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[recv_len] = '\0';
        if (strcmp(buffer, "NICK") == 0) {
            // Server is asking for nickname, main thread will handle this
        } else {
            printf("%s\n", buffer);
        }
    }
    if (recv_len <= 0) {
        printf("Disconnected from server.\n");
        closesocket(client_socket);
        exit(0);
    }
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char nickname[50];
    char message[BUFFER_SIZE];

    printf("Choose your nickname: ");
    fgets(nickname, 50, stdin);
    nickname[strcspn(nickname, "\n")] = 0; // Remove newline

    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("connect error");
        return 1;
    }
    puts("Connected");

    // Wait for NICK prompt
    char server_prompt[5];
    int recv_len = recv(client_socket, server_prompt, 4, 0);
    if (recv_len > 0) {
        server_prompt[recv_len] = '\0';
        if (strcmp(server_prompt, "NICK") == 0) {
            send(client_socket, nickname, strlen(nickname), 0);
        }
    }

    _beginthreadex(NULL, 0, &receive_messages, (void*)client_socket, 0, NULL);

    while (1) {
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline
        char full_message[BUFFER_SIZE + 52];
        sprintf(full_message, "%s: %s", nickname, message);
        if (send(client_socket, full_message, strlen(full_message), 0) < 0) {
            puts("Send failed");
            return 1;
        }
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
