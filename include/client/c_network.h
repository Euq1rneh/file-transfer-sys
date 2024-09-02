#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "c_private.h"
#include "structures.h"

/// @brief Establish a connection to the server
/// @param rtable a struct containing all the necessary information to establish connection
/// @return 0 if connection was successfull -1 otherwise
int network_connect(struct rtable_t *rtable);

/// @brief Closes the connection with the server
/// @param rtable a struct containing all the necessary information to establish connection
/// @return -1 if there was an error 0 otherwise
int network_close(struct rtable_t *rtable);
