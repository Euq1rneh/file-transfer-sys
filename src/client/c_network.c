#include "c_network.h"

int network_connect(struct rtable_t *rtable)
{
    // verificação de erros
    if (rtable == NULL || rtable->server_address == NULL || rtable->server_port <= 0)
    {
        return -1;
    }
    int sockfd;

    // Cria um socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating socket");
        return -1; // Falha ao criar o socket
    }

    // Define as informações do servidor
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(rtable->server_port);

    if (inet_pton(AF_INET, rtable->server_address, &server_addr.sin_addr) < 1)
    {
        printf("Error converting IP\n");
        close(sockfd);
        return -1;
    }

    // Estabelece a conexão com o servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error while trying to establish a connection to the server");
        close(sockfd);
        return -1; // Falha na conexão
    }

    rtable->sockfd = sockfd;
    return 0;
}

int network_close(struct rtable_t *rtable)
{
    // verificação de erros
    if (rtable == NULL || rtable->sockfd < 0)
    {
        return -1;
    }

    // Fecha o socket
    if (close(rtable->sockfd) < 0)
    {
        perror("Failed to close socket");
        return -1;
    }

    return 0;
}