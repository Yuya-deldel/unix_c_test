#define _GNU_SOURCE
#include <signal.h>
#include "_header.h"

#define BUFFSIZE 1024

static void sig_tstp(int signo) {
    sigset_t mask;

    // ...

    // unblock SIGTSTP (ハンドル中はブロックされている)
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    signal(SIGTSTP, SIG_DFL);   // デフォルトに戻す

    kill(getpid(), SIGTSTP);    // 自身にシグナル (対話的休止) を送る

    // 実行が再開しない限り return しない
    signal(SIGTSTP, sig_tstp);      // シグナルハンドラ再確立

    // ...

    return;
}

int main(void) {
    int n;
    char buf[BUFFSIZE];

    // shell が起動したプログラムはデフォルトで SIG_IGN
    // ジョブ制御を行う shell のみがシグナル動作を SIG_DFL に変更
    if (signal(SIGTSTP, SIG_IGN) == SIG_DFL) signal(SIGTSTP, sig_tstp);

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) err_sys("write error");
    }
    if (n < 0) err_sys("read error");

    exit(0);
}