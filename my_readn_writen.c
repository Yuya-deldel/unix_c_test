#include "_header.h"

// write n bytes to a descriptor
ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nwritten;
    size_t nleft = n;
    const char *ptr = vptr;     // void* -> char*
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) return nwritten;   // error

        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

ssize_t readn(int fd, void *vptr, size_t n) {
    size_t nread;
    size_t nleft = n;
    char *ptr = vptr;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            return read;    // error
        } else if (nread == 0) {
            break;      // EOF
        }

        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}