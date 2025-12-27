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

    if (fstat(fd, &statbuf) < 0) err_sys("fstat error");
    // turn on group id + turn off group execute = 必須ロック on
    if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) err_sys("fchmod error");

    tell_wait();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {   // parent
        if (write_lock(fd, 0, SEEK_SET, 0) < 0) err_sys("write_lock error");    // write lock entire file

        tell_child(pid);
        if (waitpid(pid, NULL, 0) < 0) err_sys("waitpid error");
    } else {                // child
        wait_parent();      // parent がロックするまで待つ
        set_fl(fd, O_NONBLOCK);

        if (read_lock(fd, 0, SEEK_SET, 0) != -1) err_sys("child: read_lock succeeded");
        
        printf("read_lock of already-locked region returns %d\n", errno);

        if (lseek(fd, 0, SEEK_SET) == -1) err_sys("lseek error");
        if (read(fd, buff, 2) < 0) {
            err_ret("read failed (mandatory locking works)");
        } else {
            printf("read OK (no mandatory locking), buff = %2.2s\n", buff);
        }
    }
    exit(0);
}