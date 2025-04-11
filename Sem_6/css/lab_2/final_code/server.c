#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <gmp.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_fd = INVALID_SOCKET;
    SOCKET client_sock = INVALID_SOCKET;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    int addrlen = sizeof(address);
    int result;
    
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    
    // Initialize random number generator
    srand((unsigned int)time(NULL));
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);
    
    // Bind socket
    result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (result == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    result = listen(server_fd, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    
    printf("Server is waiting for client connection...\n");
    
    // Accept connection
    client_sock = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
    printf("Client connected!\n");
    
    // Initialize GMP variables
    mpz_t p, g, private_key, public_key, client_public_key, shared_secret;
    mpz_init(p);
    mpz_init(g);
    mpz_init(private_key);
    mpz_init(public_key);
    mpz_init(client_public_key);
    mpz_init(shared_secret);
    
    // Read message from client (p and g)
    result = recv(client_sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        printf("Client says: %s\n", buffer);
        
        // Parse p and g
        char *token = strtok(buffer, " ");
        mpz_set_str(p, token, 10);
        token = strtok(NULL, " ");
        mpz_set_str(g, token, 10);
        
        gmp_printf("p = %Zd\n", p);
        gmp_printf("g = %Zd\n", g);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        closesocket(client_sock);
        closesocket(server_fd);
        WSACleanup();
        mpz_clear(p);
        mpz_clear(g);
        mpz_clear(private_key);
        mpz_clear(public_key);
        mpz_clear(client_public_key);
        mpz_clear(shared_secret);
        return 1;
    }
    
    // Generate private key (random number between 1 and 20)
    mpz_set_ui(private_key, rand() % 20 + 1);
    gmp_printf("Private key: %Zd\n", private_key);
    
    // Compute public key: g^private_key mod p
    mpz_powm(public_key, g, private_key, p);
    
    // Send public key to client
    gmp_printf("Servers public key : %Zd\n", public_key);
    memset(message, 0, BUFFER_SIZE);
    gmp_sprintf(message, "%Zd", public_key);
    send(client_sock, message, (int)strlen(message), 0);
    
    // Receive client's public key
    memset(buffer, 0, BUFFER_SIZE);
    result = recv(client_sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        mpz_set_str(client_public_key, buffer, 10);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        closesocket(client_sock);
        closesocket(server_fd);
        WSACleanup();
        mpz_clear(p);
        mpz_clear(g);
        mpz_clear(private_key);
        mpz_clear(public_key);
        mpz_clear(client_public_key);
        mpz_clear(shared_secret);
        return 1;
    }
    
    // Compute shared secret: (client_public_key)^private_key mod p
    mpz_powm(shared_secret, client_public_key, private_key, p);
    gmp_printf("Shared secret by server: %Zd\n", shared_secret);
    
    // Clean up
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(private_key);
    mpz_clear(public_key);
    mpz_clear(client_public_key);
    mpz_clear(shared_secret);
    
    closesocket(client_sock);
    closesocket(server_fd);
    WSACleanup();
    
    return 0;
}