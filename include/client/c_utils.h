#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structures.h"

/// @brief Prints a progress bar
/// @param count the current value
/// @param max the maximum value
void print_progress(size_t count, size_t max, char* progress_title);

/// @brief Reads a string of unknown size from stdin. The resulting string must be freed
/// @return the string that was read or NULL if there was an error
char *read_stdin();

/// @brief Split a string on a delimiter. This function does not handlle consecutive delimiters
/// @param str the string to split
/// @param delimiter the delimiter
/// @param size pointer to a variable where the size of the resulting array will be written
/// @return return a list a list of substrings of the original string separated by the delimiter and the size of the array in the array_size param
char** str_split(const char *str, const char *delimiter, int *size);


void clean_chunks(Chunk **chunks, int num_chunks);

/// @brief Rebuilds the original file using the chunks received from the server
/// @param chunks the array that contains all the file chunks
/// @param num_chunks number of chunks in the array
/// @return 0 if the file was successfuly rebuilt -1 otherwise
int rebuild_file(Chunk *chunks, int num_chunks, char*filepath);