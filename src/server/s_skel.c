#include "s_skel.h"

/// @brief Receives a message from the client in byte form, deserializes it and return it
/// @param client_socket client socket
/// @param expectedType the expected message type (Message, Chunk)
/// @return the deserialized message written into the according data type or NULL in case of error
void *receive(int client_socket, StructTypes expected_type)
{
    // receives the size of the message
    int msg_size = 0;
    if (receive_int(&msg_size, client_socket) == -1)
    {
        fprintf(stderr, "Error receinving msg size\n");
        return NULL;
    }
    // receives the message
    unsigned char *buffer = malloc(msg_size);
    int r = read_all(client_socket, buffer, msg_size);
    if (r == -1)
    {
        fprintf(stderr, "Error while reading message from client\n");
    }

    if (r != msg_size)
    {
        fprintf(stderr, "Size of message that was read does not have the same size and may be incomplete\n");
    }

    void *deserialized_buffer = deserialize(buffer, expected_type);

    if (deserialized_buffer == NULL)
    {
        fprintf(stderr, "There was an error while trying to deserialize the message.\n");
        return NULL;
    }

    return deserialized_buffer;
}

void send(int client_socket, void *data, size_t size)
{
    // sends the size of the message
    if (send_int(size, client_socket) == -1)
    {
        // client connection is closed
        fprintf(stderr, "Error sending message size to client\n");
        exit(-1);
    }
    // sends the message
    int r = write_all(client_socket, data, size);
    if (r == -1 || r != size)
    {
        fprintf(stderr, "Error sending message or bytes sent are different from message size\n");
        return;
    }
}

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

int handle_get(int client_socket, char *path, char *wd)
{

    if (file_exists(path) == -1)
    {
        fprintf(stderr, "File at path %s does not exist\n", path);
        send_int(-1, client_socket);
        return -1;
    }

    char *buffer;

    size_t nbytes = file_to_byte_array(path, buffer);

    if (nbytes == -1)
    {
        fprintf(stderr, "Error reading file at path %s\n", path);
        send_int(-1, client_socket);
        return -1;
    }

    fprintf(stderr, "Read %ld bytes\n", nbytes);

    Chunk **chunks;
    size_t nChunks = file_to_chunks(buffer, nbytes, chunks);

    if (nChunks == -1)
    {
        fprintf(stderr, "Error while trying to get file chunks\n");
        // TODO memory cleanup
        return -1;
    }

    // send number of chunks
    if (send_int(nChunks, client_socket) == -1)
    {
        fprintf(stderr, "Error sending number of chunks\n");
        // TODO memory cleanup
        return -1;
    }

    fprintf(stderr, "Generated %ld chunks from file\n", nChunks);

    size_t schunk_size;
    unsigned char *serialized_chunk;

    for (size_t i = 0; i < nChunks; i++)
    {
        size_t chunk_size = 0;
        serialize_chunk(chunks[i], serialized_chunk, &chunk_size);

        // send the chunk size
        unsigned char buffer[sizeof(size_t)];
        pack_size_t(buffer, chunk_size);

        send(client_socket, buffer, sizeof(size_t));

        // send the chunk
        send(client_socket, chunks[i], chunk_size);
        // wait client received msg
        int response = -1;
        if (receive_int(&response, client_socket) == -1)
        {
            fprintf(stderr, "Error retrieving client response\n");
            // TODO memory cleanup
            return -1;
        }
        // repeat
        // Memory cleanup
        free(chunks[i]->chunk);
        free(chunks[i]);
    }

    return 0;
}

int handle_client(char *wd, int sockfd)
{
    int r = 0;

    Message msg;
    size_t msg_size = 0;

    if(recv(sockfd, &msg_size, sizeof(size_t), 0) == -1){
        fprintf(stderr, "Error receiving message size\n");
        return -1;
    }

    char buffer[msg_size];

    if(recv(sockfd, &buffer, msg_size, 0) == -1){
        fprintf(stderr, "Error receiving message\n");
        return -1;
    }

    if(deserialize_message(buffer, &msg) == -1){
        fprintf(stderr, "Error deserializing message\n");
        return -1;
    }

    switch (msg.operation)
    {
    case 1: // ls comand
        handle_ls(msg.data, wd);
        break;
    case 2: // cd command

        break;
    case 3: // get command
        if (handle_get(sockfd, msg.data, wd) == -1)
        {
            fprintf(stderr, "---ERR:Could not process get request---\n");
        }
        break;
    case 10: // quit

        break;
    default:
        fprintf(stderr, "Could not process command with code %d", msg->operation);
        break;
    }

    return 0;
}
