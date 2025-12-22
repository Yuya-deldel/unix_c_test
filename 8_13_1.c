#include <signal.h>
#include "_header.h"

int main(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid != 0) {      // parent 
        sleep(2);
        exit(2);
    }

    // child 1
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid != 0) {
        sleep(4);
        abort();
    }

    // child 2 
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid != 0) {
        execl("/bin/dd", "dd", "if=/root", "of=/dev/null", NULL);
        exit(7);    // 到達しないはず
    }

    // child 3 
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid != 0) {
        sleep(8);
        exit(0);
    }

    // child 4 
    sleep(6);
    kill(getpid(), SIGKILL);
    exit(6);
}