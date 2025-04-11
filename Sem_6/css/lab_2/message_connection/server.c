#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addr_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Inform the client that connection is established
    send(new_socket, "Connection established. You can start chatting.\n", 50, 0);

    // Chat loop
    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);

        // Receive message from client
        int bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        }
        printf("Client: %s", buffer);

        // Send a reply to the client
        printf("Server: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}
