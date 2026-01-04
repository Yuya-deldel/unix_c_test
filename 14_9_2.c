#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "_header.h"

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr) {
    return (*ptr)++;
}

int main() {
    int fd, counter;
    pid_t pid;
    __caddr_t area;

    if ((fd = open("/dev/zero", O_RDWR)) < 0) err_sys("open error");
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (__caddr_t) -1) err_sys("mmap error");
    close(fd);

    tell_wait();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        for (int i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *) area)) != i) err_quit("parent: expected %d, got %d", i, counter);
            tell_child(pid);
            wait_child();
        }
    } else {
        for (int i = 1; i < NLOOPS + 1; i += 2) {
            wait_parent();
            if ((counter = update((long *) area)) != i) err_quit("child: expected %d, got %d", i, counter);
            tell_parent(getppid());
        }
    }
    exit(0);
}