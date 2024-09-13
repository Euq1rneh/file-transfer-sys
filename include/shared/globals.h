#ifndef GLOBALS_
#define GLOBALS_

#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>

/// @brief Sends an int throught the socket
/// @param num the int to send
/// @param fd the socket file descriptor
/// @return 0 if the were no errors -1 otherwise
int send_int(int num, int fd);

/// @brief Receives an int through the socket
/// @param num pointer where the result will be stored
/// @param fd the socket file descriptor
/// @return 0 if the were no errors -1 otherwise
int receive_int(int *num, int fd);

/// @brief Writes a buffer with a specified size to the fd
/// @param fd the file descriptor
/// @param buf the buffer containing the data
/// @param len the size of the buffer
/// @return the number of bytes that were written or -1 if there was an error
int write_all(int fd, void *buf, int len);


/// @brief Reads the contents of the message sent by the socket to a buffer with the specified size
/// @param fd the file descriptor
/// @param buf the buffer to write to
/// @param len the size of the buffer
/// @return the number of bytes read, -1 in case of error or 0 if EOF
int read_all(int fd, void *buf, int len); 

void pack_size_t(unsigned char *buffer, size_t value);
size_t unpack_size_t(unsigned char *buffer); 


int send_packet(char *packet, size_t buffer_size, int sockfd);
int receive_packet(char *packet, size_t buffer_size, int sockfd);
void safe_malloc(size_t size, void *buffer);

#endif
