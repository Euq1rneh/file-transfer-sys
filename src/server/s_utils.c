#include "s_utils.h"

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
        return -1;
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
    for (int i = out_ptr; i < out_ptr + size; i++)
    {
    }

    return size;
}

int file_to_chunks(char *buffer, char *path, Chunk **chunks)
{
    size_t nbytes = 0;
    if ((nbytes = file_to_byte_array(path, buffer)) == -1)
    {
    }

    int read_ptr = 0;
    int num_chunks = (nbytes + CHUNK_SIZE - 1) / CHUNK_SIZE;

    if (num_chunks < 1)
    {
        fprintf(stderr, "Number of chunks is invalid\n");
        return -1;
    }

    safe_malloc(sizeof(Chunk) * num_chunks, chunks);

    for (int i = 0; i < num_chunks; i++)
    {
        safe_malloc(sizeof(Chunk), chunks[i]);
        int to_read = num_chunks + read_ptr == nbytes ? nbytes - read_ptr : CHUNK_SIZE;

        chunks[i]->index = i;
        chunks[i]->size = to_read;

        memcpy(chunks[i]->chunk, buffer + read_ptr, to_read);
    }

    return num_chunks;
}
