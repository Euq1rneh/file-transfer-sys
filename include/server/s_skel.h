#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "s_private.h"
#include "structures.h"
#include "globals.h"


int handle_client(Operation *op, char *wd, int client_socket);

