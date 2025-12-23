#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void abort(void) {
    sigset_t mask;
    struct sigaction action;

    sigaction(SIGABRT, NULL, &action);      // action: 変更前の sigaction 構造体
    if (action.sa_handler == SIG_IGN) {     // signal を無視、のとき
        action.sa_handler = SIG_DFL;        // デフォルトに戻す
        sigaction(SIGABRT, &action, NULL);  // 変更
    }

    if (action.sa_handler == SIG_DFL) fflush(NULL);     // 開いている全ストリームで flush

    sigfillset(&mask);
    sigdelset(&mask, SIGABRT);      // SIGABRT をブロックしてはならない -> SIGABRT マスクを除去
    sigprocmask(SIG_SETMASK, &mask, NULL);

    kill(getpid(), SIGABRT);    // send the signal
    
    // これ以降は、プロセスが SIGABRT を受け取って return したあと: 後始末をして終了
    fflush(NULL);       // 開いている全ストリームで flush
    action.sa_handler = SIG_DFL;            // デフォルト設定に戻していく   
    sigaction(SIGABRT, &action, NULL);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    kill(getpid(), SIGABRT);    // 終了するはず

    exit(1);        // ここには到達しない
}