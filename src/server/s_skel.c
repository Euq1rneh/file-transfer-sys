#include "s_skel.h"

/// @brief Receives a message from the client in byte form, deserializes it and return it
/// @param client_socket client socket
/// @param expectedType the expected message type (Message, Chunk)
/// @return the deserialized message written into the according data type or NULL in case of error
void *receive(int client_socket, StructTypes expected_type)
{

}


int handle_cd(char *path, char *wd, Package *pkg)
{
    return 0;
}

/// @brief Handles the ls command
/// @param path the path to list the files
/// @param wd the working dir
/// @param pkg struct were the data is written
/// @return number of entries or -1 if there was an error
int handle_ls(char *path, char *wd, Package *pkg)
{
  return 0;
}

int handle_get(int client_socket, char *path, char *wd)
{
    if(file_exists(path) == -1){
        if(send_int(-1, client_socket) == -1){
            fprintf(stderr, "Error sending message to client\n");
            return -1;
        }        
    }

    char *file_buf;
    Chunk **chunks;

    file_to_chunks(file_buf, path, chunks);
   



    return 0;
}

int handle_client(char *wd, int sockfd)
{
    Message msg;
    size_t buffer_size = 0;
    char *buffer;

    if(receive_packet(buffer, buffer_size, sockfd) == -1){
        return -1;
    }

    deserialize_message(buffer, &msg);

    switch (msg.operation)
    {
    case 1: // ls comand
        //handle_ls(msg.data, wd);
        return -1;
        break;
    case 2: // cd command

        break;
    case 3: // get command
        if (handle_get(sockfd, msg.data, wd) == -1)
        {
            fprintf(stderr, "GET error: could not process get request\n");
        }
        break;
    case 10: // quit
        break;
    default:
        fprintf(stderr, "Could not process command with code %d", msg.operation);
        break;
    }

    return 0;
}
