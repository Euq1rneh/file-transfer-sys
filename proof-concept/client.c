#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Step 1: Create the client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Step 2: Connect to the server
    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Step 3: Create and populate the Message struct
    struct Message message;
    message.operation = 1;
    char data[] = "This is the client message. Above this message is the operation code\n";
    message.size = sizeof(data);
    message.data = data;

    // Step 4: Serialize the message into a buffer
    char *buffer;
    size_t total_size;
    serialize_message(&message, &buffer, &total_size);

    // Step 5: Send the serialized data to the server
    send(client_socket, buffer, total_size, 0);

    // Step 6: Clean up and close
    free(buffer);
    close(client_socket);

    return 0;
}
