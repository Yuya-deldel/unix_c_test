#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "_header.h"

char buf[100000];

int main(void) {
    char *ptr;

    int ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "read %d bytes\n", ntowrite);

    set_fl(STDOUT_FILENO, O_NONBLOCK);
}