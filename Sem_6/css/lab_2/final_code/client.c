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
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
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
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9001);
    
    // Convert IPv4 address from text to binary form
    result = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if (result <= 0) {
        printf("Invalid address / Address not supported\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    // Connect to server
    result = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (result == SOCKET_ERROR) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Connected to server!\n");
    
    // Initialize GMP variables for big integer operations
    mpz_t p, g, private_key, public_key, server_public_key, shared_secret;
    mpz_init(p);
    mpz_init(g);
    mpz_init(private_key);
    mpz_init(public_key);
    mpz_init(server_public_key);
    mpz_init(shared_secret);
    
    // Get prime numbers from user
    printf("Enter the two prime numbers (p and g): ");
    fgets(message, BUFFER_SIZE, stdin);
    // Remove newline if present
    message[strcspn(message, "\n")] = 0;
    
    // Send p and g to server
    send(sock, message, (int)strlen(message), 0);
    
    // Parse p and g
    char *token = strtok(message, " ");
    mpz_set_str(p, token, 10);
    token = strtok(NULL, " ");
    mpz_set_str(g, token, 10);
    
    // Generate private key (random number between 1 and 20)
    mpz_set_ui(private_key, rand() % 20 + 1);
    gmp_printf("Private key: %Zd\n", private_key);
    
    // Compute public key: g^private_key mod p
    mpz_powm(public_key, g, private_key, p);
    
    // Receive server's public key
    result = recv(sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        mpz_set_str(server_public_key, buffer, 10);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        mpz_clear(p);
        mpz_clear(g);
        mpz_clear(private_key);
        mpz_clear(public_key);
        mpz_clear(server_public_key);
        mpz_clear(shared_secret);
        return 1;
    }
    
    // Send client's public key
    gmp_printf("CLients public key : %Zd\n", public_key);
    memset(message, 0, BUFFER_SIZE);
    gmp_sprintf(message, "%Zd", public_key);
    send(sock, message, (int)strlen(message), 0);
    
    // Compute shared secret: (server_public_key)^private_key mod p
    mpz_powm(shared_secret, server_public_key, private_key, p);
    gmp_printf("Shared secret from client: %Zd\n", shared_secret);
    
    // Clean up
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(private_key);
    mpz_clear(public_key);
    mpz_clear(server_public_key);
    mpz_clear(shared_secret);
    
    closesocket(sock);
    WSACleanup();
    
    return 0;
}