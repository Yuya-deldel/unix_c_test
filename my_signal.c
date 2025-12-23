#define _GNU_SOURCE
#include <signal.h>
#include "_header.h"

Sigfunc *signal(int signo, Sigfunc *func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT 
        act.sa_flags |= SA_INTERRUPT;
#endif
        act.sa_flags |= SA_RESTART;     // 割り込まれたシステムコール (SIGALRM以外) を強制再起動
    }

    if (sigaction(signo, &act, &oact) < 0) return SIG_ERR;
    
    return oact.sa_handler;
}

// できるだけシステムコールの再起動をしない version
Sigfunc *signal_intr(int signo, Sigfunc *func) {
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (sigaction(signo, &act, &oact) < 0) return SIG_ERR;

    return oact.sa_handler;
}