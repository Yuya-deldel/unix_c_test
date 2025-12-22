#include <sys/types.h>
#include "_header.h"

int global = 6;
char buf[] = "a write to stdout\n";

int main(void) {
    int var = 88;       // in stack 
    pid_t pid;

    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) err_sys("write error");
    printf("before fork\n");     // flush しない

    if ((pid = fork()) < 0) { 
        err_sys("fork error");
    } else if (pid == 0) {      // child
        global++;
        var++;
    } else {
        sleep(2);   // parent
    }
    printf("pid = %d, global = %d, var = %d\n", getpid(), global, var);
    exit(0);    
}