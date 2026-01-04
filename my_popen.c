#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "_header.h"

static pid_t *childpid = NULL;      // file descriptor を添え字とした子の pid の配列
static int maxfd;

#define SHELL "/bin/sh"

FILE *popen(const char *cmdstring, const char *type) {
    int pfd[2];
    pid_t pid;
    FILE *fp;

    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        errno = EINVAL;
        return NULL;
    }

    if (childpid == NULL) {     // first time
        maxfd = open_max();
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL) return NULL;
    }

    if (pipe(pfd) < 0) return NULL;     // errno は pipe によって設定される
    if ((pid = fork()) < 0) {       // error
        return NULL;                    // errno は pipe によって設定される
    } else if (pid == 0) {          // child
        if (*type == 'r') {
            close(pfd[0]);              // write をクローズ
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO);    // 子の標準出力を親の fd に複製
                close(pfd[1]);          // もとの read をクローズ
            }
        } else {
            close(pfd[1]);              // read をクローズ
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDIN_FILENO);     // 子の標準入力を親の fd に複製
                close(pfd[0]);          // もとの write をクローズ
            }
        }

        for (int i = 0; i < maxfd; i++) {
            if (childpid[i] < 0) close(i);      // 子のオープン状態のストリームをすべて閉じる (仕様)
        }

        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);
        _exit(127);     // ここには到達しない
    } else {        // parent
        if (*type == 'r') {
            close(pfd[1]);      // write をクローズ
            if ((fp = fdopen(pfd[0], type)) == NULL) return NULL;   // 親の標準入力を pipe に設定
        } else {
            close(pfd[0]);      // read をクローズ
            if ((fp = fdopen(pfd[1], type)) == NULL) return NULL;   // 親の標準出力を pipe に設定
        }
        childpid[fileno(fp)] = pid;     // 子の pid を保存

        return fp;
    }
}

int pclose(FILE *fp) {
    int stat;
    pid_t pid;

    if (childpid == NULL) return -1;        // popen() が呼ばれていない

    int fd = fileno(fp);
    if ((pid = childpid[fd]) == 0) return -1;   // fp は popen() によって初期化されている = 適切な親ではない

    childpid[fd] = 0;       // childpid[fd] 初期化

    if (fclose(fp) == EOF) return -1;

    while (waitpid(pid, &stat, 0) < 0) {
        if (errno != EINTR) return -1;      // waitpid が　EINTR 以外のエラーを発した
    }

    return stat;
}