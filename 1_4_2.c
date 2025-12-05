#include "_header.h"

int main(void) {
    int c;

    while ((c = getc(stdin)) != EOF) {      // 標準入力 (stdin) から一文字 (c) 読み込む
        if (putc(c, stdout) == EOF) {       // 読み込んだ文字を書き出す
            err_sys("output error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0);
}