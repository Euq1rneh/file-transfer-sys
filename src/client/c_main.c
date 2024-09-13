#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "c_private.h"
#include "c_utils.h"
#include "c_stub.h"

void print_usage()
{
    printf("Usage: sftp_client <Server-IP> <Server-PORT>\n");
}


void user_interaction(struct rtable_t *rtable, char *initial_wd)
{
    char *working_dir = strdup(initial_wd);
    char *command;
    int array_size = 0;
    char **args;

    while (1)
    {
        printf("[ %s ] > ", working_dir);
        command = read_stdin();

        if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0)
        {
            
        }
        else if (strstr(command, "ls") != NULL)
        {
            
        }
        else if (strstr(command, "cd") != NULL)
        {
            
        }
        else
        {
            return; // there was no command
        }


        for (int i = 0; i < array_size; i++)
        {
            free(args[i]);
        }
        array_size = 0;
        free(args);
        free(command);
    }
    free(working_dir);
}

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        printf("Too many arguments!\n");
        print_usage();
        return 0;
    }

    if (argc < 3)
    {
        printf("Missing arguments!\n");
        print_usage();
    }

    struct rtable_t *table = rtable_connect(argv[1], argv[2]);

    if (table == NULL)
    {
        printf("Error while trying to establish connection to the server");
        free(table->server_address);
        free(table);
        return 0;
    }

    int wd_size = 0;

    if(receive_int(&wd_size, table->sockfd) == -1){
        printf("Error receiving server wd\n");
        exit(1);
    }

    char *homedir = malloc(wd_size);
    if(read(table->sockfd, homedir, wd_size) == -1){
        printf("Error receiving wd name\n");
        exit(1);
    }

    user_interaction(table, homedir);

    int r = rtable_disconnect(table);

    if (r == -1)
    {
        printf("There was an error while trying to disconnect from the server!\n");
        free(table->server_address);
        free(table);
        return 0;
    }

    return 0;
}
