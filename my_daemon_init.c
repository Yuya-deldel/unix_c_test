#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "_header.h"

int daemon_init(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        return -1;      // error
    } else if (pid != 0) {
        exit(0);        // parent exitted
    }   // child

    setsid();       // become session reader 
    chdir("/");     // change working directory
    umask(0);       // file mode creation mask

    return 0;
}