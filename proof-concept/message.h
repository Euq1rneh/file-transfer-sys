#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>

struct Message {
    int operation;
    size_t size;
    void *data;
};

// Functions for serializing and deserializing the message
void serialize_message(struct Message *msg, char **buffer, size_t *total_size);
void deserialize_message(struct Message *msg, char *buffer);

// Network conversion functions (for endianess)
int to_network_int(int host_int);
int to_host_int(int net_int);
size_t to_network_size(size_t host_size);
size_t to_host_size(size_t net_size);

#endif
