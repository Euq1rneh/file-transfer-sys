#include "c_utils.h"

#define SIZE 256

void print_progress(size_t count, size_t max)
{
    const int bar_width = 50;

    float progress = (float)count / max;
    int bar_length = progress * bar_width;

    printf("\rProgress: [");
    for (int i = 0; i < bar_length; ++i)
    {
        printf("#");
    }
    for (int i = bar_length; i < bar_width; ++i)
    {
        printf(" ");
    }
    printf("] %.2f%%", progress * 100);

    fflush(stdout);
}

char *read_stdin()
{
    int mem = SIZE;
    char *str = malloc(mem);
    if (str == NULL)
    {
        fprintf(stderr, "Error while allocating memory\n");
        return NULL;
    }

    char *next_char = str;

    fgets(next_char, mem, stdin);
    printf("read command\n");

    while (str[strlen(str) - 1] != '\n') // when we get whole string, last char will be '\n'
    {
        mem *= 2;
        str = realloc(str, mem);
        if (str == NULL)
        {
            fprintf(stderr, "Error while reallocating memory\n");
            return NULL;
        }

        next_char = str + strlen(str);
        fgets(next_char, mem / 2 + 1, stdin);
    }

    // chop off trailing newline from string
    *(str + strlen(str) - 1) = '\0';

    // trim mem down to exact bytes needed to hold string
    mem = strlen(str) + 1;
    str = realloc(str, mem);

    if (str == NULL)
    {
        fprintf(stderr, "Error while reallocating memory\n");
        return NULL;
    }

    return str;
}

char **str_split(const char *str, const char *delimiter, int *size)
{
    char *str_copy = strdup(str); // Make a copy of the string because strtok modifies it
    int count = 0;
    char *token;
    char **result = NULL;

    // First pass: Count the number of tokens
    token = strtok(str_copy, delimiter);
    while (token != NULL)
    {
        count++;
        token = strtok(NULL, delimiter);
    }

    // Allocate memory for the array of tokens
    result = malloc(count * sizeof(char *));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        free(str_copy);
        exit(EXIT_FAILURE);
    }

    // Second pass: Store the tokens in the array
    strcpy(str_copy, str); // Restore the original string
    token = strtok(str_copy, delimiter);
    for (int i = 0; i < count; i++)
    {
        result[i] = strdup(token); // Allocate memory for each token
        token = strtok(NULL, delimiter);
    }

    *size = count; // Set the size of the array

    free(str_copy); // Free the temporary string copy
    return result;
}