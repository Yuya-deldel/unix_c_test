#define _GNU_SOURCE
#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <unistd.h>
#include "_header.h"

// alarm() とのレースコンディションがあるため、信頼性のない実装
/*
static void sig_alarm(int signo) {
    return;     // なにもせず、 pause から覚める
}

unsigned int sleep1(unsigned int nsecs) {
    if (signal(SIGALRM, sig_alarm) == SIG_ERR) return nsecs;    // sig_alarm の設定に失敗したときの処理

    // アラームをかけて停止
    alarm(nsecs);   // 呼び出し側がアラームをセットしていた場合、解除されてしまう
    pause();        // pause() の呼び出し前に alarm が切れると永久 pause となる
    
    return alarm(0);    // アラーム取り消し
    // SIGALRM に対する動作を変更しているので元に戻す必要がある
}

///////////////////////////////////////
static jmp_buf env_alarm;
static void sig_alarm2(int signo) {
    longjmp(env_alarm, 1);
}

unsigned int sleep2(unsigned int nsecs) {
    if (signal(SIGALRM, sig_alarm) == SIG_ERR) return nsecs;

    if (setjmp(env_alarm) == 0) {   // setjmp を直接呼び出している場合 (jump してきた場合は != 0)
        alarm(nsecs);               // 途中でアラームが切れた場合はここには到達しない
        pause();
    }

    return (alarm(0));
} */

// test //////////////////
/*
static void sig_int(int signo) {
    volatile int j;

    printf("\nsig_int starting\n");
    for (int i = 0; i < 1000000000; i++) {
        j += i * i;
    }

    printf("sig_int finished\n");
    return;
}

int main(void) {
    unsigned int unslept;
    
    if (signal(SIGINT, sig_int) == SIG_ERR) err_sys("signal(SIGINT) error");

    unslept = sleep2(1);
    printf("sleep2 returned: %u\n", unslept);

    exit(0);
} */

static void sig_alarm(void) {
    return;     // do nothing
}

unsigned int sleep1(unsigned int nsecs) {
    struct sigaction newact, oldact;
    sigset_t newmask, oldmask, suspmask;
    unsigned int unslept;

    // ハンドラ設定 + 以前の状態保管
    newact.sa_handler = sig_alarm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);
    
    // マスク設定 + 以前の状態保管
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);     // SIGALRM はブロック

    alarm(nsecs);
    
    // シグナルを受信するために待機
    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);      // SIGALRM が確実にブロックされないようにする
    sigsuspend(&suspmask);      // シグナルが来るまで待機

    // 後処理
    unslept = alarm(0);     // アラーム解除: unslept は残り時間
    sigaction(SIGALRM, &oldact, NULL);          // 状態復元
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    return unslept;
}