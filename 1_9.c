#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "_header.h"

// signal catching function
void sig_int(int signo) {
    printf("interrupt\n> ");
}

int main(void) {
    char buf[MAXLINE];
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }

    printf("> ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {    // 標準入力から一行を読み込む
        buf[strlen(buf) - 1] = 0;   // replace newline with null: execlp() の引数に適合するようにする

        if ((pid = fork()) < 0) {   // 親の複製プロセスを作成 (fork()) 
            err_sys("fork error");  // pid < 0 ならば fork に失敗
        } else if (pid == 0) {      // fork() は子プロセスには pid = 0 を返す
            execlp(buf, buf, (char *) 0);   // 子プロセスを buf で入力されたコマンドを実行するプロセスに変更
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        // fork() は親プロセスには子プロセスの pid を返す
        if ((pid = waitpid(pid, &status, 0)) < 0) {     // 子プロセスの終了を待つ
            err_sys("waitpid error");                   // pid は待っている子プロセスのプロセスID
        }                                               // status は子プロセスの終了状態
        printf("> ");
    }
    exit(0);
}