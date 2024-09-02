#include "s_utils.h"

int get_code_from_command(char *command){

    if(strcmp(command, "ls"))return 1;
    else if (strcmp(command, "cd"))return 2;
    else if(strcmp(command, "get"))return 3;
    else if (strcmp(command, "put"))return 4;
    else if (strcmp(command, "quit"))return 10;
    else return -1;
}