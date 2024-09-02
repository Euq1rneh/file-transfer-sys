#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/// @brief Prints a progress bar
/// @param count the current value
/// @param max the maximum value
void print_progress(size_t count, size_t max);

/// @brief Reads a string of unknown size from stdin. The resulting string must be freed
/// @return the string that was read or NULL if there was an error
char *read_stdin();

/// @brief Split a string on a delimiter. This function does not handlle consecutive delimiters
/// @param str the string to split
/// @param delimiter the delimiter
/// @param size pointer to a variable where the size of the resulting array will be written
/// @return return a list a list of substrings of the original string separated by the delimiter and the size of the array in the array_size param
char** str_split(const char *str, const char *delimiter, int *size);

char *get_separator();