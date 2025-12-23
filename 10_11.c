#define _GNU_SOURCE
#include <signal.h>
#include "_header.h"

static void sig_quit(int signo) {
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) err_sys("can't reset SIGQUIT");
    return;
}

int main(void) {
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR) err_sys("can't catch SIGQUIT");
    
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) err_sys("SIG_BLOCK error");
    // oldmask: 現在のプロセスのシグナルマスク; newmask: SIGQUIT を加えたシグナルマスク

    sleep(5);   // ここで SIGQUIT を送る: cntl + "\"

    if (sigpending(&pendmask) < 0) err_sys("sigpending error");     // pendmask: 保留中のシグナルマスク
    if (sigismember(&pendmask, SIGQUIT)) printf("\nSIGQUIT pending\n");     // check

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) err_sys("SIG_SETMASK error"); // シグナルマスクを元に戻す
    printf("SIGQUIT unblocked\n");

    sleep(5);

    exit(0);
}