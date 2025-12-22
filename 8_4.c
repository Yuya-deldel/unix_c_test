#include <sys/types.h>
#include "_header.h"

int global = 6;

int main(void) {
    int var = 88;       // in stack 
    pid_t pid;

    printf("before fork\n");     // flush しない

    if ((pid = vfork()) < 0) { 
        err_sys("vfork error");
    } else if (pid == 0) {      // child
        global++;
        var++;
        _exit(0);
    } else {        // parent
        printf("pid = %d, global = %d, var = %d\n", getpid(), global, var);
        exit(0);    
    }
}