#include <sys/types.h>
#include "_header.h"

int main(void) {
    // current offset を調べる (-1 ならば seek 不可能)
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
        printf("cannot seek\n");
    } else {
        printf("seek OK\n");
    }
    exit(0);
}