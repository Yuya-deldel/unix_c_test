#define _GNU_SOURCE
#include <signal.h>
#include "_header.h"

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo) {
    sigflag = 1;
    return;
}

// SIGUSR1: parent tells child they've done
// SIGUSR2: child tells parent they've done

void tell_wait(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) err_sys("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) err_sys("signal(SIGUSR2) error");
    
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) err_sys("SIG_BLOCK error");
}

void tell_parent(pid_t pid) {
    kill(pid, SIGUSR2);
}

void wait_parent(void) {
    while (sigflag == 0) {
        sigsuspend(&zeromask);      // wait parent
    }

    sigflag = 0;
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) err_sys("SIG_SETMASK error");
}

void tell_child(pid_t pid) {
    kill(pid, SIGUSR1);
}

void wait_child(void) {
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }

    sigflag = 0;
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) err_sys("SIG_SETMASK error");
}