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
    SOCKET mim_server_sock = INVALID_SOCKET;
    SOCKET client_sock = INVALID_SOCKET;
    SOCKET real_server_sock = INVALID_SOCKET;
    struct sockaddr_in mim_server_addr, real_server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};
    int addrlen = sizeof(client_addr);
    int result;
    
    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    
    // Initialize random number generator
    srand((unsigned int)time(NULL));
    
    // Create MiM server socket
    mim_server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mim_server_sock == INVALID_SOCKET) {
        printf("MiM server socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    mim_server_addr.sin_family = AF_INET;
    mim_server_addr.sin_addr.s_addr = INADDR_ANY;
    mim_server_addr.sin_port = htons(9001);
    
    // Bind MiM server socket
    result = bind(mim_server_sock, (struct sockaddr *)&mim_server_addr, sizeof(mim_server_addr));
    if (result == SOCKET_ERROR) {
        printf("MiM server bind failed: %d\n", WSAGetLastError());
        closesocket(mim_server_sock);
        WSACleanup();
        return 1;
    }
    
    // Listen for client connections
    result = listen(mim_server_sock, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("MiM server listen failed: %d\n", WSAGetLastError());
        closesocket(mim_server_sock);
        WSACleanup();
        return 1;
    }
    
    // Create socket to connect to real server
    real_server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (real_server_sock == INVALID_SOCKET) {
        printf("Real server socket creation failed: %d\n", WSAGetLastError());
        closesocket(mim_server_sock);
        WSACleanup();
        return 1;
    }
    
    real_server_addr.sin_family = AF_INET;
    real_server_addr.sin_port = htons(9000);
    
    // Convert IPv4 address to binary
    result = inet_pton(AF_INET, "127.0.0.1", &real_server_addr.sin_addr);
    if (result <= 0) {
        printf("Invalid address / Address not supported\n");
        closesocket(mim_server_sock);
        closesocket(real_server_sock);
        WSACleanup();
        return 1;
    }
    
    // Connect to real server
    result = connect(real_server_sock, (struct sockaddr *)&real_server_addr, sizeof(real_server_addr));
    if (result == SOCKET_ERROR) {
        printf("Connection to real server failed: %d\n", WSAGetLastError());
        closesocket(mim_server_sock);
        closesocket(real_server_sock);
        WSACleanup();
        return 1;
    }
    printf("Connected to real server!\n");
    printf("Server is waiting for client connection...\n");
    
    // Accept connection from real client
    client_sock = accept(mim_server_sock, (struct sockaddr *)&client_addr, &addrlen);
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(mim_server_sock);
        closesocket(real_server_sock);
        WSACleanup();
        return 1;
    }
    
    // Initialize GMP variables
    mpz_t p, g, mim_private_key, mim_public_key;
    mpz_t real_server_public_key, real_client_public_key;
    mpz_t secret_shared_with_server, secret_shared_with_client;
    
    mpz_init(p);
    mpz_init(g);
    mpz_init(mim_private_key);
    mpz_init(mim_public_key);
    mpz_init(real_server_public_key);
    mpz_init(real_client_public_key);
    mpz_init(secret_shared_with_server);
    mpz_init(secret_shared_with_client);
    
    // Main MiM logic
    // Receive p,g from real client
    memset(buffer, 0, BUFFER_SIZE);
    result = recv(client_sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        
        // Forward p,g to real server
        send(real_server_sock, buffer, result, 0);
        
        // Parse p and g
        char *token = strtok(buffer, " ");
        mpz_set_str(p, token, 10);
        token = strtok(NULL, " ");
        mpz_set_str(g, token, 10);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        goto cleanup;
    }
    
    // Generate MiM private key
    mpz_set_ui(mim_private_key, rand() % 20 + 1);
    gmp_printf("Mitm private key: %Zd\n", mim_private_key);
    
    // Compute MiM public key
    mpz_powm(mim_public_key, g, mim_private_key, p);
    gmp_printf("Mim public key : %Zd\n", mim_public_key);
    
    // Get real server's public key
    memset(buffer, 0, BUFFER_SIZE);
    result = recv(real_server_sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        mpz_set_str(real_server_public_key, buffer, 10);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        goto cleanup;
    }
    
    // Send MiM public key to real server
    memset(message, 0, BUFFER_SIZE);
    gmp_sprintf(message, "%Zd", mim_public_key);
    send(real_server_sock, message, (int)strlen(message), 0);
    
    // Send MiM public key to real client
    send(client_sock, message, (int)strlen(message), 0);
    
    // Get real client's public key
    memset(buffer, 0, BUFFER_SIZE);
    result = recv(client_sock, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        buffer[result] = '\0'; // Ensure null termination
        mpz_set_str(real_client_public_key, buffer, 10);
    } else {
        printf("Receive failed: %d\n", WSAGetLastError());
        goto cleanup;
    }
    
    // Compute shared secrets
    mpz_powm(secret_shared_with_server, real_server_public_key, mim_private_key, p);
    mpz_powm(secret_shared_with_client, real_client_public_key, mim_private_key, p);
    
    gmp_printf("Secret shared with real server: %Zd\n", secret_shared_with_server);
    gmp_printf("Secret shared with real client: %Zd\n", secret_shared_with_client);
    
cleanup:
    // Clean up
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(mim_private_key);
    mpz_clear(mim_public_key);
    mpz_clear(real_server_public_key);
    mpz_clear(real_client_public_key);
    mpz_clear(secret_shared_with_server);
    mpz_clear(secret_shared_with_client);
    
    closesocket(client_sock);
    closesocket(mim_server_sock);
    closesocket(real_server_sock);
    WSACleanup();
    
    return 0;
}