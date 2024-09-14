#include "c_stub.h"

struct rtable_t *rtable_connect(char *address, char *port)
{
    if (address == NULL || port == NULL)
    {
        return NULL;
    }

    int p = atoi(port);
    struct rtable_t *rtable = (struct rtable_t *)malloc(sizeof(struct rtable_t));

    if (rtable == NULL)
    {
        return NULL;
    }

    rtable->server_address = strdup(address);
    rtable->server_port = p;

    if (network_connect(rtable) == -1)
    {
        free(rtable->server_address);
        free(rtable);
        return NULL;
    }

    return rtable;
}

int rtable_disconnect(struct rtable_t *rtable)
{
    if (rtable == NULL)
    {
        return -1;
    }

    if (network_close(rtable) == -1)
    {
        if (rtable->server_address != NULL)
        {
            free(rtable->server_address);
        }
        free(rtable);
        return -1;
    }

    if (rtable->server_address != NULL)
    {
        free(rtable->server_address);
    }

    free(rtable);

    return 0;
}

char *change_working_dir(char *path, struct rtable_t *rtable)
{
    return NULL;
}

int list_files(char *path, char *working_dir, struct rtable_t *rtable)
{
    return -1;
}

int get_file(char *path, struct rtable_t *rtable)
{
    if (path == NULL)
    {
        fprintf(stderr, "No file path specified\n");
        return -1;
    }

    Message msg;

    size_t path_len = strlen(path);

    msg.operation = 3;
    msg.size = path_len;
    msg.data = path;

    char *buffer;
    size_t buffer_size = sizeof(int) + sizeof(size_t) + path_len;
    safe_malloc(buffer_size, buffer);
    serialize_message(&msg, buffer, buffer_size);

    if (send_packet(buffer, buffer_size, rtable->sockfd) == -1)
    {
        return -1;
    }
    free(buffer);

    int num_chunks = 0;
    int progress_count = 0;

    if (receive_int(&num_chunks, rtable->sockfd) == -1)
    {
        fprintf(stderr, "Error could not receive data from server: could not receive number of chunks\n");
        return -1;
    }

    if(num_chunks == -1){
        fprintf(stderr, "File does not exist\n");
        return -1;
    }

    Chunk **chunks;
    safe_malloc(sizeof(Chunk) * num_chunks, chunks);
    print_progress(progress_count, num_chunks, "Chunks Received");

    for (int i = 0; i < num_chunks; i++)
    {
        char *buffer;
        size_t buffer_size;
        if (receive_packet(buffer, buffer_size, rtable->sockfd) == -1)
        {
            return -1;
        }

        Chunk *c;
        safe_malloc(sizeof(Chunk), c);
        deserialize_chunk(c, buffer);
        chunks[i] = c;
        progress_count++;

        print_progress(progress_count, num_chunks, "Chunks received");
    }

    if (rebuild_file(chunks, num_chunks, "test") == -1)
    {
        return -1;
    }

    clean_chunks(chunks, num_chunks);

    return -1;
}