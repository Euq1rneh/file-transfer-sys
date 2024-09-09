#include "message.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

// Converts an int from host to network byte order
int to_network_int(int host_int)
{
    return htonl(host_int);
}

// Converts an int from network to host byte order
int to_host_int(int net_int)
{
    return ntohl(net_int);
}

// Converts size_t from host to network byte order
size_t to_network_size(size_t host_size)
{
#if SIZE_MAX > 0xFFFFFFFF
    return htobe64(host_size);
#else
    return htonl(host_size);
#endif
}

// Converts size_t from network to host byte order
size_t to_host_size(size_t net_size)
{
#if SIZE_MAX > 0xFFFFFFFF
    return be64toh(net_size);
#else
    return ntohl(net_size);
#endif
}

// Serializes the message into a buffer for sending
void serialize_message(struct Message *msg, char **buffer, size_t *total_size)
{
    *total_size = sizeof(int) + sizeof(size_t) + msg->size;
    *buffer = malloc(*total_size);

    int operation_net = to_network_int(msg->operation);
    size_t size_net = to_network_size(msg->size);

    // Copy operation and size into buffer
    memcpy(*buffer, &operation_net, sizeof(int));
    memcpy(*buffer + sizeof(int), &size_net, sizeof(size_t));

    // Copy the data into the buffer
    memcpy(*buffer + sizeof(int) + sizeof(size_t), msg->data, msg->size);
}

// Deserializes a message from the received buffer
void deserialize_message(struct Message *msg, char *buffer)
{
    int operation_net;
    size_t size_net;   

    // Extract the operation and size from the buffer
    memcpy(&operation_net, buffer, sizeof(int));
    memcpy(&size_net, buffer + sizeof(int), sizeof(size_t));


    msg->operation = to_host_int(operation_net);
    msg->size = to_host_size(size_net);

    // Allocate memory for the data and copy it from the buffer
    msg->data = malloc(msg->size);
    memcpy(msg->data, buffer + sizeof(int) + sizeof(size_t), msg->size);
}
