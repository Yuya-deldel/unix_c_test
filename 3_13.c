#include <sys/types.h>
#include <fcntl.h>
#include "_header.h"

int main(int argc, char *argv[]) {
    int val;

    if (argc != 2) err_quit("usage: ./a.out <descriptor#>");
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {     // ファイル状態フラグ取得
        err_sys("fcntl error for fd %d", atoi(argv[1]));
    }

    int accmode = val & O_ACCMODE;      // access mode にする
    if (accmode == O_RDONLY) printf("read only");
    else if (accmode == O_WRONLY) printf("write only");
    else if (accmode == O_RDWR) printf("read write");
    else err_dump("unknown access mode");

    if (val & O_APPEND) printf(", append");
    if (val & O_NONBLOCK) printf(", nonblocking");
    if (val & O_SYNC) printf(", synchronous writes");
    putchar('\n');
    
    exit(0);
}