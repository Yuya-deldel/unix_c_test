#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "_header.h"

#define PIDFILE "daemon.pid"
#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH     // -rw-r--r--

int main(void) {
    int fd, val;
    char buf[10];

    if ((fd = open(PIDFILE, O_WRONLY | O_CREAT, FILE_MODE)) < 0) err_sys("open error");
    
    if (write_lock(fd, 0, SEEK_SET, 0) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            exit(0);    // daemon is already running
        } else {
            err_sys("write_lock error");
        }
    }   // PIDFILE に write lock がかかった状態

    // pid を書き込む
    if (ftruncate(fd, 0) < 0) err_sys("ftruncate error");
    sprintf(buf, "%d\n", getpid());
    if (write(fd, buf, strlen(buf)) != strlen(buf)) err_sys("write error");

    // close-on-exec flag を立てる
    if ((val = fcntl(fd, F_GETFD, 0)) < 0) err_sys("fcntl F_GETFD error");
    val |= FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, val) < 0) err_sys("fcntl F_SETFD error");

    // 終了するまでファイルを開けたままにする: ロック状態が継続する

    exit(0);
}