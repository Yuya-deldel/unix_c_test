#include <sys/types.h>
#include <sys/wait.h>
#include "_header.h"

int main(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {      // first child
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid > 0) {   // parent of second child
            exit(0);            
        }
        // second child: parent has changed to init
        // 最初の親が終了した後でもゾンビにならない 
        sleep(2); 
        printf("second child, parent pid = %d\n", getppid());
        exit(0);
    }
    // first parent
    if (waitpid(pid, NULL, 0) != pid) err_sys("waitpid error");     // wait for first child

    exit(0);
}