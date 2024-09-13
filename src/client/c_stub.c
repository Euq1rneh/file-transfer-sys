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

    // Fechar a conexão com o servidor
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
    return NULL;
}

int list_files(char *path, char *working_dir, struct rtable_t *rtable)
{
    return -1;
}

int get_file(char *path, struct rtable_t *rtable)
{
    return -1;
}