#include <sys/types.h>
#include "_header.h"

// バッファなしで文字列を一文字ずつ stdout に出力
static void charatatime(char *str) {
    int c;

    setbuf(stdout, NULL);       // unbuffered
    for (char* ptr = str; c = *ptr++;) {
        putc(c, stdout);
    }
}

int main(void) {
    pid_t pid;

//    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
//        WAIT_PARENT();
        charatatime("output from child\n");
    } else {
        charatatime("output from parent\n");
//        TELL_CHILD(pid);
    }
    exit(0);
}