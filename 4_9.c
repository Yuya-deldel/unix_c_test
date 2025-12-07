#include <sys/types.h>
#include <sys/stat.h>
#include "_header.h"

int main(void) {
    struct stat statbuf;
    if (stat("foo.txt", &statbuf) < 0) err_sys("stat error for foo.txt");

    if (chmod("foo.txt", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) err_sys("chmod error for foo.txt");
    if (chmod("bar.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) err_sys("chmod error for bar.txt");

    exit(0);
}