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
    Message msg;
    msg.operation = 2;

    if(path == NULL){
        msg.data = "";
        msg.size = 0;
    }else{
        msg.data = path;
        msg.size = strlen(path) + 1;
    }

    // SENDING MESSAGE
    size_t total_size = sizeof(int) + sizeof(size_t) + msg.size;
    char *buffer = malloc(total_size);
    
    serialize_message(&msg, buffer, &total_size);

    total_size = to_network_size(total_size);

    if(send(rtable->sockfd, &total_size, sizeof(size_t), 0) == -1){
        fprintf(stderr, "Error sending message size\n");
        return NULL;
    }

    if(send(rtable->sockfd, buffer, total_size, 0) == -1){
        fprintf(stderr, "Error sending message to server\n");
        return NULL;
    }

    //TODO memory cleanup from the serialization

    //RECEIVING MESSAGE

    size_t recv_msg_size = 0;

    if(recv(rtable->sockfd, recv_msg_size, sizeof(size_t), 0) == -1){
        fprintf(stderr, "Error receiving message size from server\n");
        return NULL;
    }

    char buffer[recv_msg_size];

    if(recv(rtable->sockfd, &buffer, recv_msg_size, 0) == -1){
        fprintf(stderr, "Error receiving message from server\n");
        return NULL;
    }

    deserialize_message(buffer, &msg);


    char *new_wd = malloc(sizeof(char) * msg.size);

    strcpy(new_wd, (char*)msg.data);

    //TODO memory cleanup from deserializing message

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
