#include "serialization.h"

// Converts an int from host to network byte order
int to_network_int(int host_int) {
    return htonl(host_int);
}

// Converts an int from network to host byte order
int to_host_int(int net_int) {
    return ntohl(net_int);
}

// Converts a uint64_t from host to network byte order
uint64_t to_network_uint64(uint64_t host_uint64) {
    #if SIZE_MAX > 0xFFFFFFFF
        return htobe64(host_uint64);
    #else
        return htonl(host_uint64); // Fallback to 32-bit on 32-bit systems
    #endif
}

// Converts a uint64_t from network to host byte order
uint64_t to_host_uint64(uint64_t net_uint64) {
    #if SIZE_MAX > 0xFFFFFFFF
        return be64toh(net_uint64);
    #else
        return ntohl(net_uint64); // Fallback to 32-bit on 32-bit systems
    #endif
}

// Converts size_t from host to network byte order
size_t to_network_size(size_t host_size) {
    #if SIZE_MAX > 0xFFFFFFFF
        return htobe64(host_size);
    #else
        return htonl(host_size);
    #endif
}

// Converts size_t from network to host byte order
size_t to_host_size(size_t net_size) {
    #if SIZE_MAX > 0xFFFFFFFF
        return be64toh(net_size);
    #else
        return ntohl(net_size);
    #endif
}
void *deserialize(void *data, StructTypes expected_Type)
{
    size_t size;
    switch (expected_Type)
    {
    case ST_CHUNK:
        Chunk *chunk;
        deserialize_chunk(data, chunk);

        return chunk;
        break;
    case ST_MESSAGE:
        Message *msg;
        size = deserialize_message(data, msg);

        if (size == -1)
        {
            fprintf(stderr, "Error deserializing message\n");
            return NULL;
        }
        return msg;
        break;
    }
}

size_t serialize_message(Message *msg, char *buffer, size_t messag_size)
{
    int operation_net = to_network_int(msg->operation);
    size_t size_net = to_network_size(msg->size);

    // Copy operation and size into buffer
    memcpy(*buffer, &operation_net, sizeof(int));
    memcpy(*buffer + sizeof(int), &size_net, sizeof(size_t));
    
    // Copy the data into the buffer
    memcpy(*buffer + sizeof(int) + sizeof(size_t), msg->data, msg->size);
}

void serialize_chunk(struct Chunk *chunk, char **buffer, size_t *total_size) {
    *total_size = sizeof(int) + sizeof(uint64_t) + chunk->size;
    *buffer = malloc(*total_size);

    int index_net = to_network_int(chunk->index);
    uint64_t size_net = to_network_uint64(chunk->size);

    // Copy index and size into the buffer
    memcpy(*buffer, &index_net, sizeof(int));
    memcpy(*buffer + sizeof(int), &size_net, sizeof(uint64_t));

    // Copy the actual chunk data into the buffer
    memcpy(*buffer + sizeof(int) + sizeof(uint64_t), chunk->chunk, chunk->size);
}

void deserialize_chunk(struct Chunk *chunk, char *buffer) {
    int index_net;
    uint64_t size_net;

    // Extract the index and size from the buffer
    memcpy(&index_net, buffer, sizeof(int));
    memcpy(&size_net, buffer + sizeof(int), sizeof(uint64_t));

    chunk->index = to_host_int(index_net);
    chunk->size = to_host_uint64(size_net);

    // Copy the actual chunk data from the buffer
    memcpy(chunk->chunk, buffer + sizeof(int) + sizeof(uint64_t), chunk->size);
}

int deserialize_int(uint8_t *buffer)
{
    uint32_t net_order;
    memcpy(&net_order, buffer, sizeof(net_order));  // Copy from buffer
    return ntohl(net_order);  // Convert back to host byte order
}

size_t deserialize_message(unsigned char *data, Message *msg)
{
    int operation_net;
    size_t size_net;

    // Extract the operation and size from the buffer
    memcpy(&operation_net, data, sizeof(int));
    memcpy(&size_net, data + sizeof(int), sizeof(size_t));

    msg->operation = to_host_int(operation_net);
    msg->size = to_host_size(size_net);

    // Allocate memory for the data and copy it from the buffer
    msg->data = malloc(msg->size);
    memcpy(msg->data, data + sizeof(int) + sizeof(size_t), msg->size);

    return 0;
}