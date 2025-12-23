#include <sys/types.h>
#include <signal.h>
#include "_header.h"

static void sig_int(int signo) {
    printf("caught SIGINT\n");
    return;
}

static void sig_chld(int signo) {
    printf("caught SIGCHLD\n");
    return;
}

int main(void) {
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR) err_sys("signal(SIGINT) error");
    if (signal(SIGCHLD, sig_chld) == SIG_ERR) err_sys("signal(SIGCHLD) error");

    if ((status = system("/bin/ed")) < 0) err_sys("system() error");

    exit(0);
}