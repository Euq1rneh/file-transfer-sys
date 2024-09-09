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

    // Fŵechar a conexão com o servidor
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
    Message *msg = malloc(sizeof(struct Message));

    msg->operation = 2;

    if(path == NULL){
        msg->data = malloc(1);

        if(msg->data == -1){
            fprintf(stderr, "Error allocating memory. Exiting program...\n");
            exit(-1);
        }

        msg->size = 0;
    }else{
        size_t path_size = strlen(path);
        msg->data = malloc(sizeof(char) * (path_size + 1));

        if(msg->data == -1){
            fprintf(stderr, "Error allocating memory. Exiting program...\n");
            exit(-1);
        }

        msg->size = path_size + 1;
    }

    serialize_message(msg, buffer, msg_size);

    if(write_all(rtable->sockfd, msg, sizeof(struct Message)) == -1){
        fprintf(stderr, "Error while trying to write message to server. Closing program\n");
        exit(-1);
    }

    free(msg->data);

    int r = read_all(rtable->sockfd, msg, sizeof(struct Message));

    if(r == -1){
        fprintf(stderr, "Error while trying to read message from server. Discarding message\n");
        return NULL;
    }else if(r == 0){
        fprintf(stderr, "EOF detected while reading message from server\n");
    }

    char *new_wd = malloc(sizeof(char) * msg->size);

    strcpy(new_wd, (char*)msg->data);

    //need to free msg

    return new_wd;
}

int list_files(char *path, char *working_dir, struct rtable_t *rtable)
{

}

//              CD

    // DIR *dir_stream;
    // char *new_path;

    // if (path == NULL)
    // {
    //     struct passwd *pw = getpwuid(getuid());
    //     const char *homedir = pw->pw_dir;

    //     char *new_wd = strdup(homedir);
    //     return new_wd;
    // }

    // if (strcmp(path, "..") == 0)
    // {

    //     if (strcmp(working_dir, "/") == 0)
    //     {
    //         return NULL;
    //     }

    //     // if num occurrence of '/' = 1 return "/" as the new path
    //     char *first_separator = strchr(working_dir, kPathSeparator[0]);
    //     char *last_separator = strrchr(working_dir, kPathSeparator[0]); // get the last occurrence of the file separator

    //     if (last_separator == NULL || first_separator == NULL)
    //     {
    //         printf("There was an error while trying to change working directory");
    //         return NULL;
    //     }

    //     int first_index = first_separator - working_dir;
    //     int last_index = last_separator - working_dir;

    //     // is at root of the file system
    //     if (first_index == last_index)
    //     {
    //         new_path = malloc(sizeof(char) * 2);
    //         new_path = strncpy(new_path, working_dir, 1);
    //         new_path[1] = '\0';

    //         return new_path;
    //     }

    //     new_path = malloc(sizeof(char) * (last_index + 1)); // index starts at zero and the '\0'

    //     if (new_path == NULL)
    //     {
    //         printf("Error while trying to change working dir");
    //         return NULL;
    //     }

    //     new_path = strncpy(new_path, working_dir, last_index);
    //     new_path[last_index] = '\0';

    //     return new_path;
    // }

    // new_path = strdup(path);

    // dir_stream = opendir(new_path); // try path to see if is a absolute path

    // if (dir_stream == NULL)
    // {
    //     free(new_path);
    //     new_path = malloc(sizeof(char) * (strlen(path) + strlen(working_dir) + strlen(kPathSeparator) + 1));
    //     new_path = strcpy(new_path, working_dir);
    //     new_path = strcat(new_path, kPathSeparator);
    //     new_path = strcat(new_path, path);

    //     dir_stream = opendir(new_path); // test for a folder already in the working dir
    //     if (dir_stream == NULL)
    //     {
    //         printf("No such file or directory\n");
    //         free(new_path);
    //         return NULL;
    //     }
    //     return new_path;
    // }


//              LS

// DIR *dir_stream;
    // struct dirent *dir_read;
    // struct stat mystat;
    // char buf[512];
    // char *new_path;

    // if (path == NULL)
    // {
    //     new_path = strdup(working_dir);
    // }
    // else
    // {
    //     new_path = strdup(path);
    // }

    // dir_stream = opendir(new_path); // try path to see if is a absolute path

    // if (dir_stream == NULL)
    // {
    //     free(new_path);
    //     new_path = malloc(sizeof(char) * (strlen(path) + strlen(working_dir) + strlen(kPathSeparator) + 1));
    //     new_path = strcpy(new_path, working_dir);
    //     new_path = strcat(new_path, kPathSeparator);
    //     new_path = strcat(new_path, path);

    //     dir_stream = opendir(new_path); // test for a folder already in the working dir
    //     if (dir_stream == NULL)
    //     {
    //         printf("No such file or directory\n");
    //         free(new_path);
    //         return -1;
    //     }
    // }

    // printf("%s\n", new_path);
    // printf("│\n");

    // // read the contents of dir:
    // while ((dir_read = readdir(dir_stream)) != NULL)
    // {
    //     sprintf(buf, "%s/%s", new_path, dir_read->d_name);
    //     stat(buf, &mystat);

    //     if (strcmp(dir_read->d_name, ".") == 0 || strcmp(dir_read->d_name, "..") == 0)
    //     {
    //         printf("├─ %s \n", dir_read->d_name);
    //     }
    //     else
    //     {
    //         printf("├─ %s \t\t\t%ld bytes\n", dir_read->d_name, mystat.st_size);
    //     }
    // }

    // // close the directory
    // if (closedir(dir_stream) == -1)
    // {
    //     perror("Can't close the dir \n");
    //     free(new_path);
    //     return -1;
    // }

    // free(new_path);
