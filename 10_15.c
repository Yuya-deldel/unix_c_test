#define _GNU_SOURCE
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include "_header.h"

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;

static void sig_usr1(int signo) {
    time_t starttime;

    if (canjump == 0) return;       // unexpected signal, ignore

    pr_mask("starting sig_usr1: ");
    alarm(3);

    starttime = time(NULL);
    for (;;) {      // 5sec 待機
        if (time(NULL) > starttime + 5) break;
    }

    pr_mask("finishing sig_usr1: ");
    
    canjump = 0;
    siglongjmp(jmpbuf, 1);
}

static void sig_alarm(int signo) {
    pr_mask("in sig_alarm: ");
    return;
}

int main(void) {
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR) err_sys("signal(SIGUSR1) error");
    if (signal(SIGALRM, sig_alarm) == SIG_ERR) err_sys("signal(SIGALRM) error");
    pr_mask("starting main: ");

    if (sigsetjmp(jmpbuf, 1)) {     // siglongjmp から来た場合
        pr_mask("ending main: ");
        exit(0);
    }

    canjump = 1;
    for (;;) pause();       // 待機
}