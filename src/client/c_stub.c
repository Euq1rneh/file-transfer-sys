#include "c_stub.h"

struct rtable_t *rtable_connect(char *address, char *port)
{
    // verificação de erros
    if (address == NULL || port == NULL)
    {
        return NULL;
    }

    // converção para inteiro
    int p = atoi(port);
    // alocar memória para rtable
    struct rtable_t *rtable = (struct rtable_t *)malloc(sizeof(struct rtable_t));

    // verificação de erros
    if (rtable == NULL)
    {
        return NULL;
    }

    // Preencher a estrutura rtable com as informações necessárias
    rtable->server_address = strdup(address);
    rtable->server_port = p;

    // Estabelecer a conexão com o servidor
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
    // verificação de erros
    if (rtable == NULL)
    {
        return -1;
    }

    // Fechar a conexão com o servidor
    if (network_close(rtable) == -1)
    {
        if (rtable->server_address != NULL)
        {
            free(rtable->server_address);
        }
        free(rtable);
        return -1;
    }

    // destruição de memória alocada
    if (rtable->server_address != NULL)
    {
        free(rtable->server_address);
    }

    // destruição de memória alocada
    free(rtable);

    return 0;
}

char *change_working_dir(char *path, struct rtable_t *rtable)
{
    Message msg;
    msg.operation = 2;

    if (path == NULL)
    {
        msg.data = "";
        msg.size = 0;
    }
    else
    {
        msg.data = path;
        msg.size = strlen(path) + 1;
    }

    // SENDING MESSAGE
    size_t total_size = sizeof(int) + sizeof(size_t) + msg.size;
    char *buffer = malloc(total_size);

    serialize_message(&msg, buffer, &total_size);

    total_size = to_network_size(total_size);

    if (send(rtable->sockfd, &total_size, sizeof(size_t), 0) == -1)
    {
        fprintf(stderr, "Error sending message size\n");
        return NULL;
    }

    if (send(rtable->sockfd, buffer, total_size, 0) == -1)
    {
        fprintf(stderr, "Error sending message to server\n");
        return NULL;
    }

    // TODO memory cleanup from the serialization

    // RECEIVING MESSAGE

    size_t recv_msg_size = 0;

    if (recv(rtable->sockfd, recv_msg_size, sizeof(size_t), 0) == -1)
    {
        fprintf(stderr, "Error receiving message size from server\n");
        return NULL;
    }

    char buffer[recv_msg_size];

    if (recv(rtable->sockfd, &buffer, recv_msg_size, 0) == -1)
    {
        fprintf(stderr, "Error receiving message from server\n");
        return NULL;
    }

    deserialize_message(buffer, &msg);

    char *new_wd = malloc(sizeof(char) * msg.size);

    strcpy(new_wd, (char *)msg.data);

    // TODO memory cleanup from deserializing message

    return new_wd;
}

int list_files(char *path, char *working_dir, struct rtable_t *rtable)
{
}

int get_file(char *path, struct rtable_t *rtable)
{

    if (path == NULL || strcmp(path, "") == 0 || strcmp(path, " ") == 0)
    {
        return -1;
    }

    Message msg;
    msg.operation = 2;
    msg.size = strlen(path) + 1;
    msg.data = path;

    // SENDING MESSAGE
    size_t total_size = sizeof(int) + sizeof(size_t) + msg.size;
    char *buffer = malloc(total_size);

    serialize_message(&msg, buffer, &total_size);

    total_size = to_network_size(total_size);
    // sending message size
    if (send(rtable->sockfd, &total_size, sizeof(size_t), 0) == -1)
    {
        fprintf(stderr, "Error sending message size\n");
        return NULL;
    }
    // sending message
    if (send(rtable->sockfd, buffer, total_size, 0) == -1)
    {
        fprintf(stderr, "Error sending message to server\n");
        return NULL;
    }

    // TODO memory cleanup from the serialization

    // RECEIVING MESSAGE
    int num_chunks = 0;
    if (receive_int(&num_chunks, rtable->sockfd) == -1)
    {
        fprintf(stderr, "Error receiving file information from server\n");
        return -1;
    }

    if (num_chunks == -1)
    {
        fprintf(stderr, "Error receiving file information from server\n");
        return -1;
    }

    Chunk chunks[num_chunks];
    uint64_t file_size = 0;

    for (int i = 0; i < num_chunks; i++)
    {
        // receive chunk size
        size_t current_chunk_size = 0;
        if (recv(rtable->sockfd, &current_chunk_size, sizeof(size_t), 0) == -1)
        {
            fprintf(stderr, "Error receiving file chunk size\n");
            return -1;
        }
        // receive chunk
        char buffer[current_chunk_size];

        if (recv(rtable->sockfd, &buffer, current_chunk_size, 0) == -1)
        {
            fprintf(stderr, "Error reading file stream. Aborting file transfer...\n");
            return -1;
        }

        // deserialized chunk
        Chunk current_chunk;
        deserialize_chunk(&current_chunk, buffer);
        // add it to chunk array
        chunks[i] = current_chunk;
        // calculate total file size based on each chunk size
        fprintf(stderr, "Updating file size...\n");
        file_size += current_chunk.size;
        fprintf(stderr, "New file size: %ld\n", file_size);
    }

    // rebuild file based on array of chunks and file size
    if (rebuild_file(chunks, num_chunks, "test") == -1) // retrieve original file name from path parameter
    {
        fprintf(stderr, "Error rebuilding file\n");
        return -1;
    }
    // free all the memory used for recreating the file (chunk array)

    return 0;
}