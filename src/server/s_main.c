#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "s_network.h"



void print_usage(){
    printf("Usage: sftp_server <PORT>\n");
}

int main(int argc, char const *argv[])
{
    // Usage: sftp_server <IP> <PORT>

    if(argc > 2){
        printf("Too many arguments!\n");
        print_usage();
        return 0;
    }

    if(argc < 2){
        printf("Missing arguments!\n");
        print_usage();
    }

    int port = atoi(argv[1]);

    //inicialização do socket
    int l_socket = network_server_init(port);

    //verificação de erros
    if (l_socket == -1)
    {
        printf("Bad port number");
        exit(2);
    }

    //get HOME dir
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    char *wd = strdup(homedir);

    if (network_main_loop(l_socket, wd) == -1)
    {
        if (network_server_close(l_socket) == -1)
        {
            perror("Error while closing socket");
        }
        perror("Error in main loop");
        return -1;
    }
    
    network_server_close(l_socket);

    return 0;
}
