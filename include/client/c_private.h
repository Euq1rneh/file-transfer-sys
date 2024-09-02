#ifndef C_PRIVATE_
#define C_PRIVATE_

struct rtable_t {
    char *server_address;
    int server_port;
    int sockfd;
};

#endif