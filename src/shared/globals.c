#include "globals.h"

int send_int(int num, int fd)
{
    //fprintf(stderr, "init send_int\n");
    int32_t conv = htonl(num);
    char *data = (char*)&conv;
    int left = sizeof(conv);
    int rc;
    do {
        rc = write(fd, data, left);
        if (rc < 0) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be writable again
            }
            else if (errno != EINTR) {
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);

    //fprintf(stderr, "end send_int\n");
    return 0;
}

int receive_int(int *num, int fd)
{
    int32_t ret;
    char *data = (char*)&ret;
    int left = sizeof(ret);
    int rc;
    do {
        rc = read(fd, data, left);
        if (rc <= 0) { /* instead of ret */
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be readable again
            }
            else if (errno != EINTR) {
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);
    *num = ntohl(ret);
    return 0;
}

int write_all(int fd, int *buf, int len){
    int bufsize = len;
    while (len > 0){
        int res = write(fd, buf, len);
        if (res < 0){
            if (errno == EINTR)
                continue;
            return res;
        }
        if (res == 0)
            return res;
        buf += res;
        len -= res;
    }
    return bufsize;
}

int read_all(int fd, int *buf, int len) {
    int bufsize = len;
    while (len > 0) {
        int res = read(fd, buf, len);
        if (res < 0) {
            if (errno == EINTR)
                continue;
            return res; 
        }
        if (res == 0)
            return res;
        buf += res;
        len -= res;
    }
    return bufsize;
}