#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "c_private.h"
#include "c_network.h"
#include "globals.h"
#include "structures.h"


/// @brief Attemps to connect to the server
/// @param address the server IP address
/// @param port the server port
/// @return a struct containing all the important information for managing the connection
struct rtable_t *rtable_connect(char *address, char *port);


/// @brief Attemps to disconnect to the server
/// @param rtable a struct containing all the information for managing the connection
/// @return 0 if successfuly disconected -1 otherwise
int rtable_disconnect(struct rtable_t *rtable);

/// @brief Change working directory
/// @param rtable a struct containing all the information for managing the connection
/// @return the path to the new working dir
char *change_working_dir(char *path, struct rtable_t *rtable);

/// @brief List the files at path
/// @param path the directory to list the files from. If NULL it will list from the current working dir
/// @param working_dir a pointer to the path of the current working dir
/// @param rtable table containing information about the server connection
/// @return -1 if there was an error 0 otherwise
int list_files(char *path, char *working_dir, struct rtable_t *rtable);