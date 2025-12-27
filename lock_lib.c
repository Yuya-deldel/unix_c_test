#include <sys/types.h>
#include <fcntl.h>
#include "_header.h"

int lock_leg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
    struct flock lock;

    lock.l_type = type;     // F_RDLCK, F_WRLCK, F_UNLCK
    lock.l_start = offset;
    lock.l_whence = whence; // SEEK_SET, SEEK_CUR, SEEK_END
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len) {
    struct flock lock;

    lock.l_type = type;         // F_RDLCK, F_WRLCK
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    if (fcntl(fd, F_GETLK, &lock) < 0) err_sys("fcntl error");  // lock にロック情報取得
    
    if (lock.l_type == F_UNLCK) {
        return 0;   // 領域はロックされていない
    } else {
        return lock.l_pid;  // ロックしているプロセスの pid
    }
}