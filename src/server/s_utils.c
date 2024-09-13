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
    return -1;
}

size_t file_to_chunks(char *buffer, size_t buffer_size, Chunk **chunks)
{
    return -1;
}
