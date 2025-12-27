#include <termios.h>
#include "_header.h"

int main(void) {
    struct termios term;
    long vdisable;

    // isatty(): file descriptor が端末と関連付けられているか？
    if (isatty(STDIN_FILENO) == 0) err_quit("standard input is not a terminal device");
    // 端末特殊文字無効化フラグを取得
    if (vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE) < 0) 
        err_quit("fpathconf error or _POSIX_VDISABLE not in effect");
    // 標準入力に結びついている端末の情報 (termios structure) を取得
    if (tcgetattr(STDIN_FILENO, &term) < 0) err_sys("tcgetattr error");

    term.c_cc[VINTR] = vdisable;    // 割り込み文字無効化
    term.c_cc[VEOF] = 2;            // EOF を cntl + B に変更

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0) err_sys("tcsetattr error");

    exit(0);
}