#include <fcntl.h>
#include "_header.h"

void set_fl(int fd, int flags) {
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) err_sys("fcntl F_GETFL error");

    // flag を on にする
    val |= flags;
    if (fcntl(fd, F_SETFL, val) < 0) err_sys("fcntl F_SETFL error");
}

void clr_fl(int fd, int flags) {
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) err_sys("fcntl F_GETFL error");

    // flag を off にする
    val &= ~flags;
    if (fcntl(fd, F_SETFL, val) < 0) err_sys("fcntl F_SETFL error");
}