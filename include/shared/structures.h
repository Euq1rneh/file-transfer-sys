#ifndef STRUCTURES_
#define STRUCTURES_

#include <stdio.h>

// this struct is used to tell the server and client some usefull data 
// before executing a request
typedef struct Package
{
   size_t size;        //stores the size of anything that is needed e.g args, array dir_structure, etc...
   char *dir_name;
   char *filename;         //filename for file transfer purposes
   char **dir_structure;    //array containing the directory structure for the ls command
} Package ;

#endif