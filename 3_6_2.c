#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "_header.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
mode_t FILE_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;   // -rw-r--r--

int main(void) {
    int fd;
    if ((fd = open("file.hole", O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("creat error");
    }

    if (write(fd, buf1, 10) != 10) {
        err_sys("buf1 write error");
    }   // offset = 10

    if (lseek(fd, 40, SEEK_SET) == -1) {
        err_sys("lseek error");
    }   // offset = 40

    if (write(fd, buf2, 10) != 10) {
        err_sys("buf2 write error");
    }   // offset = 50

    exit(0);
}