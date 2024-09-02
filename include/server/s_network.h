#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "globals.h"
#include "structures.h"
#include "s_private.h"
#include "s_skel.h"
#include "s_utils.h"

/// @brief Initializes the server socket
/// @param port the port to open the server
/// @return 0 if server initialization was successful -1 otherwise
int network_server_init(short port);

/// @brief The server main loop that handles creating new threads for clients and their requests
/// @param listening_socket the file descriptor for the server socket
/// @return -1 if there was an error 0 otherwise
int network_main_loop(int listening_socket, char *wd);

/// @brief Closes the server socket
/// @param socket a file descriptor for the socket
/// @return 0 if closing was successful -1 otherwise
int network_server_close(int socket);