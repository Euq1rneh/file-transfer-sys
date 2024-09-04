#include "s_network.h"

int network_server_init(short port)
{
    int sockfd;
    // Cria socket TCP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error while tryin to create socket");
        return -1;
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Error in setsockpt");
        exit(200);
    }

    // Preenche estrutura server para bind
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error while trying to bind");
        close(sockfd);
        return -1;
    }

    // listen
    if (listen(sockfd, 0) < 0)
    {
        perror("Error while tryin to listen");
        close(sockfd);
        return -1;
    }

    printf("Server ready, wainting for connections\n");

    return sockfd;
}

int network_server_close(int socket)
{
    // verificação de erros e fecha socket do servidor
    if (close(socket) < 0)
    {
        perror("Failed to close socket");
        return -1;
    }
    return 0;
}

void *network_receive(int client_socket)
{
   

}



int network_main_loop(int listening_socket, char *wd)
{
    int connsockfd;
    struct sockaddr_in client;
    socklen_t size_client = sizeof(client);

    // Bloqueia a espera de pedidos de conexão
    while ((connsockfd = accept(listening_socket, (struct sockaddr *)&client, &size_client)) != -1)
    {
        printf("Client connection established\n");
        // loop de pedidos do cliente aceite

        printf("Client connection closed\n");
        close(connsockfd);
    }

    if (connsockfd == -1)
    {
        perror("Error accepting connection");
        return -1;
    }

    return 0;
}
