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

int write_all(int fd, void *buf, int len){
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

int read_all(int fd, void *buf, int len) {
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


void pack_size_t(unsigned char *buffer, size_t value) {
    for (int i = sizeof(size_t) - 1; i >= 0; --i) {
        buffer[i] = value & 0xFF;
        value >>= 8;
    }
}

size_t unpack_size_t(unsigned char *buffer) {
    size_t value = 0;
    for (int i = 0; i < sizeof(size_t); ++i) {
        value = (value << 8) | buffer[i];
    }
    return value;
}

// uint64_t uint64_to_networkByteOrder(uint64_t value) {
//     if (__BYTE_ORDER == __LITTLE_ENDIAN) {
//         return ((uint64_t)htonl(value & 0xFFFFFFFF) << 32) | htonl(value >> 32);
//     } else {
//         return value;
//     }
// }

// uint64_t networkByteOrder_to_uint64(uint64_t value) {
//     if (__BYTE_ORDER == __LITTLE_ENDIAN) {
//         return ((uint64_t)ntohl(value & 0xFFFFFFFF) << 32) | ntohl(value >> 32);
//     } else {
//         return value;
//     }
// }