#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

/*  シグナルを処理しないナイーブな実装
int system(const char *cmdstring) {
    pid_t pid;
    int status;

    if (cmdstring == NULL) return 1;        // system() は使用可能

    if ((pid = fork()) < 0) {
        status = -1;    // fork error
    } else if (pid == 0) {      // child
        // -c: コマンド行引数をコマンド入力として扱う
        execl("/bin/sh", "sh", "-c", cmdstring, (char *) 0);    // /bin/sh のプログラムを実行
        _exit(127);     // 子プロセス側で標準入出力バッファをフラッシュしないようにする
    } else {        // parent
        while (waitpid(pid, &status, 0) < 0) {      // waitpid error
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return status;
} */

// SIGINT, SIGQUIT を無視し、 SIGCHLD をブロックしなければならない
int system(const char *cmdstring) {
    pid_t pid;
    int status;
    struct sigaction ignore, saveintr, savequit;
    sigset_t chldmask, savemask;

    if (cmdstring == NULL) return 1;    // system() が使えるかどうかの判定 (使える)

    // SIGINT, SIGQUIT を無視
    ignore.sa_handler = SIG_IGN;    
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if (sigaction(SIGINT, &ignore, &saveintr) < 0) return -1;
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0) return -1;

    // SIGCHLD をブロック
    sigemptyset(&chldmask);
    sigaddset(&chldmask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0) return -1;

    // コマンドを実行
    if ((pid = fork()) < 0) {
        status = -1;    // fork error
    } else if (pid == 0) {      // child
        // 以前のシグナルに戻す + シグナルマスクリセット
        // SIGINT, SIGQUIT を無視する + SIGCHLD をブロックするのは親のみ
        sigaction(SIGINT, &saveintr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        // -c: コマンド行引数をコマンド入力として扱う
        execl("/bin/sh", "sh", "-c", cmdstring, (char *) 0);    // /bin/sh のプログラムを実行
        _exit(127);     // 子プロセス側で標準入出力バッファをフラッシュしないようにする
    } else {        // parent
        while (waitpid(pid, &status, 0) < 0) {      // waitpid error
            if (errno != EINTR) {
                status = -1;        
                break;
            }
        }
    }

    // 以前のシグナルに戻す + シグナルマスクリセット
    if (sigaction(SIGINT, &saveintr, NULL) < 0) return -1;
    if (sigaction(SIGQUIT, &savequit, NULL) < 0) return -1;
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0) return -1;

    return status;
}