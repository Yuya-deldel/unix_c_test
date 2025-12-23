#include <pwd.h>
#include <signal.h>
#include "_header.h"

// 未定義動作する例

static void my_alarm(int signo) {
    struct passwd *rootptr;

    printf("in signal handler\n");
    // シグナルハンドラ中で再入不可関数を呼ぶ (getpwnam)
    if ((rootptr = getpwnam("root")) == NULL) err_sys("getpwnam(root) error");

    alarm(1);
    return;
}

int main(void) {
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);

    for (;;) {
        if ((ptr = getpwnam("deltagraviton")) == NULL) err_sys("getpwnam error");
        if (strcmp(ptr->pw_name, "deltagraviton") != 0) {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
        }
    }
}