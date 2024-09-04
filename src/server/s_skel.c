#include "s_skel.h"

int handle_cd(char *path, char *wd, Package *pkg)
{
    DIR *dir_stream;
    char *new_path;

    if (path == NULL)
    {
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;

        // char *new_wd = strdup(homedir);
        pkg->dir_name = strdup(homedir);

        return 0;
    }

    printf("Path sent by client: %s\n", path);

    if (strcmp(path, "..") == 0)
    {
        printf("Trying to go back a dir\n");
        if (strcmp(wd, "/") == 0)
        {
            pkg->dir_name = strdup(wd);
            return 0;
        }

        // if num occurrence of '/' = 1 return "/" as the new path
        char *first_separator = strchr(wd, '/');
        char *last_separator = strrchr(wd, '/'); // get the last occurrence of the file separator

        if (last_separator == NULL || first_separator == NULL)
        {
            printf("There was an error while trying to change working directory");
            return -1;
        }

        int first_index = first_separator - wd;
        int last_index = last_separator - wd;

        // is at root of the file system
        if (first_index == last_index)
        {
            new_path = malloc(sizeof(char) * 2);
            new_path = strncpy(new_path, wd, 1);
            new_path[1] = '\0';

            pkg->dir_name = strdup(new_path);

            free(new_path);
            return 0;
        }

        new_path = malloc(sizeof(char) * (last_index + 1)); // index starts at zero and the '\0'

        if (new_path == NULL)
        {
            printf("Error while trying to change working dir");
            return -1;
        }

        new_path = strncpy(new_path, wd, last_index);
        new_path[last_index] = '\0';

        pkg->dir_name = strdup(new_path);

        free(new_path);

        return 0;
    }

    new_path = strdup(path);

    dir_stream = opendir(new_path); // try path to see if is a absolute path

    if (dir_stream == NULL)
    {
        free(new_path);
        new_path = malloc(sizeof(char) * (strlen(path) + strlen(wd) + 1 + 1));
        new_path = strcpy(new_path, wd);
        new_path = strcat(new_path, "/");
        new_path = strcat(new_path, path);

        dir_stream = opendir(new_path); // test for a folder already in the working dir
        if (dir_stream == NULL)
        {
            printf("No such file or directory\n");
            free(new_path);
            return 0;
        }
        return 0;
    }

    return 0;
}

/// @brief Handles the ls command
/// @param path the path to list the files
/// @param wd the working dir
/// @param pkg struct were the data is written
/// @return number of entries or -1 if there was an error
int handle_ls(char *path, char *wd, Package *pkg)
{
    if (pkg == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    DIR *dir_stream;
    struct dirent *dir_read;
    struct stat mystat;
    char buf[512];
    char *new_path;

    if (path == NULL)
    {
        fprintf(stderr, "path is NULL");
        new_path = strdup(wd);
    }
    else
    {
        fprintf(stderr, "path is NOT NULL");
        new_path = strdup(path);
    }

    dir_stream = opendir(new_path); // try path to see if is a absolute path

    if (dir_stream == NULL)
    {
        free(new_path);
        new_path = malloc(sizeof(char) * (strlen(path) + strlen(wd) + 1 + 1));
        new_path = strcpy(new_path, wd);
        new_path = strcat(new_path, "/");
        new_path = strcat(new_path, path);

        dir_stream = opendir(new_path); // test for a folder already in the working dir
        if (dir_stream == NULL)
        {
            printf("No such file or directory\n");
            free(new_path);
            return -1;
        }
    }

    // count the num of entries in a dir path
    int entries = 1;
    int capacity = 10;

    pkg->dir_structure = malloc(sizeof(char *) * capacity);

    if (pkg->dir_structure == NULL)
    {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    // printf("%s\n│\n", new_path);

    char head[strlen(new_path) + 4 + 1];
    sprintf(head, "%s\n│\n", new_path);

    pkg->dir_structure[0] = malloc(sizeof(char) * (strlen(head) + 1));
    strcpy(pkg->dir_structure[0], head);

    while ((dir_read = readdir(dir_stream)) != NULL)
    {

        if (entries == capacity)
        {
            capacity *= 2; // Double the capacity
            char **temp = realloc(pkg->dir_structure, capacity * sizeof(char *));
            if (temp == NULL)
            {
                printf("Reallocation failed!\n");
                // Free already allocated memory before exiting
                for (int j = 0; j < entries; j++)
                {
                    free(pkg->dir_structure[j]);
                }
                free(pkg->dir_structure);
                return 1;
            }
            pkg->dir_structure = temp;
        }

        // Allocate memory for the string and copy it to the array
        sprintf(buf, "%s/%s", new_path, dir_read->d_name);
        stat(buf, &mystat);

        if (strcmp(dir_read->d_name, ".") == 0 || strcmp(dir_read->d_name, "..") == 0)
        {
            // printf("├─ %s \n", dir_read->d_name);

            // ├─ len = 6 + space x 2 + \n = 6 + 2 + 1
            pkg->dir_structure[entries] = malloc(sizeof(char) * (strlen(dir_read->d_name) + 9 + 1));
            sprintf(pkg->dir_structure[entries], "├─ %s \n", dir_read->d_name);

            if (pkg->dir_structure[entries] == NULL)
            {
                printf("Memory allocation for string failed!\n");
                // Free already allocated memory before exiting
                for (int j = 0; j < entries; j++)
                {
                    free(pkg->dir_structure[j]);
                }
                free(pkg->dir_structure);
                return 1;
            }
        }
        else
        {
            // printf("├─ %s \t\t\t%ld bytes\n", dir_read->d_name, mystat.st_size);
            // ├─ len = 6 + (space x 3) + (3 x \t) + sizeof(mystat.st_size) +5 (word -> bytes) + \n = 6 + 3 + 3 + 5 + 1 = 18

            pkg->dir_structure[entries] = malloc(sizeof(mystat.st_size) + sizeof(char) * (+strlen(dir_read->d_name) + 18 + 1));

            if (pkg->dir_structure[entries] == NULL)
            {
                printf("Memory allocation for string failed!\n");
                // Free already allocated memory before exiting
                for (int j = 0; j < entries; j++)
                {
                    free(pkg->dir_structure[j]);
                }
                free(pkg->dir_structure);
                return 1;
            }

            sprintf(pkg->dir_structure[entries], "├─ %s \t\t\t%ld bytes\n", dir_read->d_name, mystat.st_size);
        }
        entries++;
    }

    // Trim the array to fit the exact number of elements
    char **trimmedArray = realloc(pkg->dir_structure, entries * sizeof(char *));
    if (trimmedArray != NULL)
    {
        pkg->dir_structure = trimmedArray;
    }
    else
    {
        printf("Trimming array failed, using untrimmed array.\n");
    }

    // close the directory
    if (closedir(dir_stream) == -1)
    {
        perror("Can't close the dir \n");
        free(new_path);
        return -1;
    }

    free(new_path);
    return entries;
}


int handle_get(char *path, char *wd){

    if(file_exists(path) == -1){
        fprintf(stderr, "File at path %s does not exist\n", path);
        return -1;
    }

    char *buffer;

    size_t nbytes = file_to_byte_array(path, buffer);

    if(nbytes == -1){
        fprintf(stderr, "Error reading file at path %s\n", path);
        return -1;
    }

    fprintf(stderr, "Read %ld bytes\n", nbytes);

    Chunk **chunks;
    size_t nChunks = file_to_chunks(buffer, nbytes, chunks);

    if(nChunks == -1){
        fprintf(stderr, "Error while trying to get file chunks\n");
        //TODO memory cleanup
        return -1;
    }

    fprintf(stderr, "Generated %ld chunks from file\n", nChunks);

    size_t schunk_size;
    unsigned char* serialized_chunk;

    // Serialize each chunk
    for (size_t i = 0; i < nChunks; i++)
    {
        schunk_size = serialize_chunk(chunks[i], serialized_chunk);
        //send the chunk size
        //send the chunk

        //wait client received msg
        //repeat

        // Memory cleanup
        free(chunks[i]->chunk);
        free(chunks[i]);
    }
    

    // Send each chunk individually

}


int handle_client(Operation *op, char *wd, int sockfd)
{
    char *path = NULL;
    Package *pkg;

    switch (op->op_code)
    {
    case 1: // ls comand
        break;

    case 2: // cd command

        break;
    case 3: // get command
        /* code */
        break;
    case 4: // put command

        break;
    case 10: // quit

        break;
    default:
        break;
    }

    return 0;
}
