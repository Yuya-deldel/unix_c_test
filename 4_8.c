#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "_header.h"

int main(void) {
    umask(0);   // super_user
    if (open("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) {
        err_sys("creat error for foo");
    }

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (open("bar.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) {
        err_sys("creat error for bar");
    }

    exit(0);
}