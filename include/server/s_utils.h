#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "structures.h"
#include "globals.h"

int copy_to_from(void *dst, void *src, int src_start, size_t size);

int get_code_from_command(char *command);

int file_exists(char *path);

size_t file_to_byte_array(char *path, char *buffer);

size_t get_file_size(FILE *fd);

int file_to_chunks(char *buffer, char *path, Chunk **chunks);