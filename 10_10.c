#include <setjmp.h>
#include <signal.h>
#include "_header.h"

/*
static void sig_alarm(int signo) {
    return;
}

int main(void) {
    int n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alarm) == SIG_ERR) err_sys("signal(SIGALRM) error");

    alarm(10);  // 期限時間設定
    // read() が実行されるまでに alarm が切れる可能性がある
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) err_sys("read error");
    alarm(0);   // アラーム解除

    write(STDOUT_FILENO, line, n);

    exit(0);
}
*/
static jmp_buf env_alarm;
static void sig_alarm(int signo) {
    longjmp(env_alarm, 1);      // 他のシグナルハンドラと干渉しうる
}

int main(void) {
    int n;
    char line[MAXLINE];

    if (signal(SIGALRM, sig_alarm) == SIG_ERR) err_sys("signal(SIGALRM) error");
    if (setjmp(env_alarm) != 0) err_quit("read timeout");   // jump してきた場合、エラー終了

    alarm(10);  // 期限時間設定
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) err_sys("read error");
    alarm(0);   // アラーム解除

    write(STDOUT_FILENO, line, n);

    exit(0);
}