#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "_header.h"

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH     // -rw-r--r--

static void lockabyte(const char *name, int fd, off_t offset) {
    if (writew_lock(fd, offset, SEEK_SET, 1) < 0) err_sys("%s: writew_lock error", name);

    printf("%s: got the lock, byte %ld\n", name, offset);
}

int main(void) {
    int fd;
    pid_t pid;
    
    if (fd = open("templock", O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE) < 0) err_sys("open error");
    
    if (write(fd, "ab", 2) != 2) err_sys("write error");

    tell_wait();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        lockabyte("child", fd, 0);
        tell_parent(getppid());
        wait_parent();
        lockabyte("child", fd, 1);
    } else {
        lockabyte("parent", fd, 1);
        tell_child(pid);
        wait_child();
        lockabyte("parent", fd, 0);
    }

    exit(0);
}