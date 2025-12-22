#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "_header.h"

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
}