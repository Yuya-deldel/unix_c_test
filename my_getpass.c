// getpass(): 端末でユーザーがタイプしたパスワードを読む関数: login(), crypt() が呼ぶ
// 端末をカノニカルモードにしておき、エコーを止めて、タイプされたパスワードを読む
#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <termios.h>

#define MAX_PASS_LEN 8  // password の最大長

char *getpass(const char *prompt) {
    static char buf[MAX_PASS_LEN + 1];
    char *ptr;
    sigset_t sig, sigsave;
    struct termios term, termsave;
    FILE *fp;

    if ((fp = fopen(ctermid(NULL), "r+")) == NULL) return NULL;     // 制御端末を開く
    setbuf(fp, NULL);       // バッファ停止 (書き出しと読み出しとの間の相互作用を切る)

    // SIGINT, SIGTSTP をブロック
    sigemptyset(&sig);
    sigaddset(&sig, SIGINT);
    sigaddset(&sig, SIGTSTP);
    sigprocmask(SIG_BLOCK, &sig, &sigsave);

    tcgetattr(fileno(fp), &termsave);   // termios 構造体取得
    term = termsave;                    // copy
    term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(fileno(fp), TCSAFLUSH, &term);    // エコーを止める

    fputs(prompt, fp);  // タイプされたパスワードを (行単位で) fp に出力

    // 一文字ずつ *ptr に収める
    ptr = buf;
    int c;
    while ((c = getc(fp)) != EOF && c != '\n') {
        if (ptr < &buf[MAX_PASS_LEN]) *ptr++ = c;
    }
    *ptr = 0;
    putc('\n', fp);

    // 状態復元
    tcsetattr(fileno(fp), TCSAFLUSH, &termsave);
    sigprocmask(SIG_SETMASK, &sigsave, NULL);
    fclose(fp);

    return buf;
}

// test
#include "_header.h"

int main(void) {
    char *ptr;
    if ((ptr = getpass("Enter password:")) == NULL) err_sys("getpass error");
    printf("password: %s\n", ptr);

    while (*ptr != 0) *ptr++ = 0;

    exit(0);
}