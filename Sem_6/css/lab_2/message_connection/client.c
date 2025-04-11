#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000
#define MAX_BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Receive the welcome message from the server
    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);

    // Chat loop
    while (1) {
        printf("Client: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Send the message to the server
        send(sock, buffer, strlen(buffer), 0);

        // Receive the server's reply
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytes_received = read(sock, buffer, sizeof(buffer));
        if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        }
        printf("Server: %s", buffer);
    }

    // Close the socket
    close(sock);
    return 0;
}
