#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // Para a função close
#include <arpa/inet.h> // Para a função inet_ntoa

#define PORT 8080
#define MAX_REQUEST_SIZE 1024

void handle_client(int client_socket) {
    char buffer[MAX_REQUEST_SIZE];
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><head><title>Simple Web Server</title></head><body><h1>Welcome to my simple web server!</h1></body></html>";

    // Read request from client
    recv(client_socket, buffer, MAX_REQUEST_SIZE, 0);
    printf("Request received:\n%s\n", buffer);

    // Send response to client
    send(client_socket, response, strlen(response), 0);

    // Close socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Set server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Handle client request
        handle_client(client_socket);
    }

    // Close server socket
    close(server_socket);

    return 0;
}