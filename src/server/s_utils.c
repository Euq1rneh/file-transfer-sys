#include "s_utils.h"

/// @brief Copies all the elements from src to dst starting at a given index
/// @param dst destination
/// @param src source
/// @param src_start index to start writting in src
/// @return
int copy_to_from(void *dst, void *src, int src_start, size_t size)
{
    if (dst == NULL || src == NULL || src_start < 0)
    {
        fprintf(stderr, "Incompatible arguments.\n");
        return -1;
    }

    void *start = src + src_start;
    // does not check if the dst if the memory allocated is equal to  size
    memcpy(dst, start, size);

    return 0;
}

int get_code_from_command(char *command)
{

    if (strcmp(command, "ls"))
        return 1;
    else if (strcmp(command, "cd"))
        return 2;
    else if (strcmp(command, "get"))
        return 3;
    else if (strcmp(command, "put"))
        return 4;
    else if (strcmp(command, "quit"))
        return 10;
    else
        return -1;
}

int file_exists(char *path)
{
    return access(path, F_OK) == 0 ? 0 : -1;
}

size_t file_to_byte_array(char *path, char *buffer)
{
    FILE *fileptr = fopen(path, "rb"); // Open the file in binary mode
    if (fileptr == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    size_t size = get_file_size(fileptr);

    // Read the file into the existing buffer
    fread(buffer, size, 1, fileptr);
    fclose(fileptr);

    return size;
}

size_t get_file_size(FILE *fd)
{
    fseek(fd, 0L, SEEK_END);
    size_t size = ftell(fd);
    rewind(fd);

    return size;
}

/// @brief Writes a file chunk
/// @param in the chunk buffer
/// @param out the buffer to read from
/// @param size the size to read
/// @param out_ptr the pointer to the last read position in the out buffer
/// @return the size of the chunk
size_t write_chunk(char *in, char *out, int size, int out_ptr)
{
    in = malloc(sizeof(char) * size);

    if (in == -1)
    {
        fprintf(stderr, "Error allocating memory\n");
        return -1;
    }

    int remaining = size;

    while (remaining > 0)
    {
        in[remaining] = out[out_ptr];
        remaining--;
        out_ptr++;
    }

    return size;
}

size_t file_to_chunks(char *buffer, size_t buffer_size, Chunk **chunks)
{
    size_t num_chunks = (buffer_size / CHUNK_SIZE) + 1;

    chunks = malloc(sizeof(struct Chunk *) * num_chunks);

    if (chunks == -1)
    {
        fprintf(stderr, "Error allocating memory\n");
        return -1;
    }

    int buff_pos = 0;
    int remaining_buffer_size = buffer_size;

    int size = 0;

    for (size_t i = 0; i < num_chunks; i++)
    {
        chunks[i] = malloc(sizeof(Chunk));

        if (chunks[i] == -1)
        {
            fprintf(stderr, "Error allocating memory\n");
            // TODO memory cleanup
            return -1;
        }

        size = remaining_buffer_size > CHUNK_SIZE ? CHUNK_SIZE : remaining_buffer_size; // the size of the current chunk
        chunks[i]->index = i;
        chunks[i]->size = write_chunk((char *)chunks[i]->chunk, buffer, size, buff_pos); // writes the chunk

        if (chunks[i]->size == -1)
        {
            fprintf(stderr, "Error chunking file\n");
            // TODO memory cleanup
            return -1;
        }

        remaining_buffer_size -= size; // calculate the remaining buffer size
    }

    return num_chunks;
}
