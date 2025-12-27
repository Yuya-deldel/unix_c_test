// 端末を cbreak / raw mode にする関数
#include <termio.h>
#include <unistd.h>

static struct termios save_termios;
static int ttysavefd = -1;
static enum {RESET, RAW, CBREAK} ttystate = RESET;

int tty_cbreak(int fd) {
    struct termios buf;

    if (tcgetattr(fd, &save_termios) < 0) return -1;
    buf = save_termios;     // buf に fd の端末属性をコピー
    buf.c_lflag &= ~(ECHO | ICANON);    // echo とカノニカルモード off
    buf.c_cc[VMIN] = 1;     // read は 1byte 受け取るまで戻らない (いつまでもブロックする)
    buf.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0) return -1;      // 設定

    ttystate = CBREAK;
    ttysavefd = fd;
    return 0;
}

int tty_raw(int fd) {
    struct termios buf;
    
    if (tcgetattr(fd, &save_termios) < 0) return -1;
    buf = save_termios;     // buf に fd の端末属性をコピー
    buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    buf.c_cflag &= ~(CSIZE | PARENB);
    buf.c_cflag |= CS8;
    buf.c_oflag &= ~(OPOST);
    buf.c_cc[VMIN] = 1;     // read は 1byte 受け取るまで戻らない (いつまでもブロックする)
    buf.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0) return -1;      // 設定

    ttystate = RAW;
    ttysavefd = fd;
    return 0;
}

int tty_reset(int fd) {
    if (ttystate != CBREAK && ttystate != RAW) return 0;
    
    if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0) return -1;      // 元に戻す

    ttystate = RESET;
    return 0;
}

void tty_atexit(void) {
    if (ttysavefd >= 0) tty_reset(ttysavefd);
}

struct termios *tty_termios(void) {     // もとの tty 状態を返す
    return &save_termios;
}

// test 
#include <signal.h>
#include "_header.h"

static void sig_catch(int signo) {
    printf("signal catched\n");
    tty_reset(STDIN_FILENO);

    exit(0);
}

int main(void) {
    int i;
    char c;

    // signal 設定
    if (signal(SIGINT, sig_catch) == SIG_ERR) err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_catch) == SIG_ERR) err_sys("signal(SIGQUIT) error");
    if (signal(SIGTERM, sig_catch) == SIG_ERR) err_sys("signal(SIGTERM) error");

    // raw mode test
    if (tty_raw(STDIN_FILENO) < 0) err_sys("tty_raw error");    
    printf("Enter raw mode characters, terminate with DELETE\n");

    while ((i = read(STDIN_FILENO, &c, 1)) == 1) {  // 標準入力から読み取り
        if ((c &= 255) == 0177) break;      // ASCII DELETE
        printf("%o\n", c);
    }

    if (tty_reset(STDIN_FILENO) < 0) err_sys("tty_reset error");
    if (i <= 0) err_sys("read error");

    // cbreak mode test 
    if (tty_cbreak(STDIN_FILENO) < 0) err_sys("tty_cbreak error");
    printf("\nEnter cbreak mode characters, terminate with SIGINT\n");

    while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
        c &= 255;
        printf("%o\n", c);
    }
    
    tty_reset(STDIN_FILENO);
    if (i <= 0) err_sys("read error");

    exit(0);
}