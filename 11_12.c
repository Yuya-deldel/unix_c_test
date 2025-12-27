#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "_header.h"

static void pr_winsize(int fd) {
    // ウィンドウのサイズ情報を取得
    struct winsize size;
    if (ioctl(fd, TIOCGWINSZ, (char *) &size) < 0) err_sys("TIOCGWINSZ error");     

    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo) {
    printf("SIGWINCH received\n");
    pr_winsize(STDIN_FILENO);
    return;
}

int main(void) {
    if (isatty(STDIN_FILENO) == 0) exit(1);     // 標準入力と接続している端末はあるか？

    // サイズが変更されると SIGWINCH シグナルが送られるので、処理する (デフォルトでは無視)
    if (signal(SIGWINCH, sig_winch) == SIG_ERR) err_sys("signal error");

    pr_winsize(STDIN_FILENO);
    
    for (;;) pause();   // sleep forever
}