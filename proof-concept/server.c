#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_client(int client_socket) {
    // Step 1: Receive the operation and size fields
    size_t header_size = sizeof(int) + sizeof(size_t);
    char header_buffer[header_size];
    
    recv(client_socket, header_buffer, header_size, 0);

    // Step 2: Deserialize the message (header part first)
    struct Message received_message;
    deserialize_message(&received_message, header_buffer);

    // Step 3: Receive the actual data based on size
    received_message.data = malloc(received_message.size);
    recv(client_socket, received_message.data, received_message.size, 0);

    // Step 4: Output the received message
    printf("Received operation: %d\n", received_message.operation);
    printf("Received data: %s\n", (char *)received_message.data);

    // Step 5: Free allocated memory
    free(received_message.data);
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Step 1: Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Step 2: Bind the socket
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Step 3: Listen for incoming connections
    listen(server_socket, 1);

    // Step 4: Accept a client connection
    printf("Waiting for a client to connect...\n");
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

    // Step 5: Handle the client connection
    handle_client(client_socket);

    // Close the server socket
    close(server_socket);
    return 0;
}
