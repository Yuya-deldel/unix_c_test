#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "_header.h"

char buf[10000];

int main(void) {
    char *ptr;

    int ntowrite = read(STDIN_FILENO, buf, sizeof(buf));    // ntowrite: 読み取った byte 数
    fprintf(stderr, "read %d bytes\n", ntowrite);

    set_fl(STDOUT_FILENO, O_NONBLOCK);      // set_fl: フラグを設定 -> nonblocking

    for (ptr = buf; ntowrite > 0;) {
        errno = 0;
        int nwrite = write(STDOUT_FILENO, ptr, ntowrite);   // nwrite: 書き込んだ byte 数
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);
        if (nwrite > 0) {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }

    clr_fl(STDOUT_FILENO, O_NONBLOCK);      // clear nonblocking
    exit(0);
}