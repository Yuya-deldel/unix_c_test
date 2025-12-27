#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "_header.h"

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH     // -rw-r--r--

int main(void) {
    int fd;

    if ((fd = open("temp.txt", O_RDONLY | O_CREAT | O_TRUNC, FILE_MODE)) < 0) err_sys("open error");

    for (int i = 0; i < 1000000; i++) {
        if (writew_lock(fd, 0, SEEK_END, 0) < 0) err_sys("writew_lock error");
        if (write(fd, &fd, 1) != 1) err_sys("write error");
        if (un_lock(fd, 0, SEEK_END, 0) < 0) err_sys("un_lock error");
        if (write(fd, &fd, 1) != 1) err_sys("write error");
    }

    exit(0);
}