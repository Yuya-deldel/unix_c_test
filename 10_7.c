#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

static void sig_cld(void) {
    pid_t pid;
    int status;

    printf("SIGCLD received\n");
    if (signal(SIGCLD, sig_cld) == -1) perror("signal error");  // ハンドラ再設定

    if ((pid = wait(&status)) < 0) perror("wait errror");   // child の状態取得
    printf("pid = %d\n", pid);
    return;
}

int main(void) {
    pid_t pid;
    
    if (signal(SIGCLD, sig_cld) == -1) perror("signal error");
    if ((pid = fork()) < 0) perror("fork error");
    
    else if (pid == 0) {    // child
        sleep(2);
        _exit(0);
    }

    // parent 
    pause();
    exit(0);
}