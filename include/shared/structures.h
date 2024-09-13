#ifndef STRUCTURES_
#define STRUCTURES_

#include <stdio.h>
#include <stdint.h>

#define CHUNK_SIZE 512000 // 500KiB

typedef enum StructTypes{
   ST_MESSAGE,
   ST_CHUNK
} StructTypes;

// this struct is used to tell the server and client some usefull data 
// before executing a request
typedef struct Package
{
   size_t size;        //stores the size of anything that is needed e.g args, array dir_structure, etc...
   char *dir_name;
   char *filename;         //filename for file transfer purposes
   char **dir_structure;    //array containing the directory structure for the ls command
} Package ;


typedef struct Message{

   int operation;
   size_t size;
   void *data;

} Message;

/// @brief Represents file chunks and is used for file transfers
typedef struct Chunk{
   int index; // index of the chunk
   uint64_t size; // actual size of the chunk in case one chunk is smaller than the max size
   unsigned char chunk[CHUNK_SIZE];
} Chunk;

#endif