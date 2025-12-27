#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "_header.h"

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH     // -rw-r--r--

int main(void) {
    int fd;
    pid_t pid;
    char buff[5];
    struct stat statbuf;

    if ((fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) err_sys("open error");
    if (write(fd, "abcdef", 6) != 6) err_sys("write error");
}