#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include "_header.h"

static void err_doit(int errnoflag, const char *fmt, va_list ap) {
    int errno_save = errno;     // vsprintf, fflush などは errno を変更する可能性があるので一時保存
    char buf[MAXLINE];
    
    vsprintf(buf, fmt, ap);
    if (errnoflag) sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
    return;
}

// Nonfatal error related to a system call. Print a message and return.
void err_ret(const char *fmt, ...) {
    va_list ap;         // 可変長引数のおまじない
    va_start(ap, fmt);  // ap 初期化 (fmt は可変な変数の直前の引数)

    err_doit(1, fmt, ap);
    
    va_end(ap);         // 可変長引数のおまじない

    return;
}

// Fatal error related to a system call. Print a message and terminate.
void err_sys(const char *fmt, ...) {
    va_list ap;        
    va_start(ap, fmt); 

    err_doit(1, fmt, ap);
    
    va_end(ap);        

    exit(1);
}

// Fatal error related to a system call. Print a message, dump core, and terminate.
void err_dump(const char *fmt, ...) {
    va_list ap;        
    va_start(ap, fmt); 

    err_doit(1, fmt, ap);
    
    va_end(ap);

    abort();    // dump core and terminate.
    exit(1);    // shouldn't get here
}

// Nonfatal error unrelated to a system call. Print a message and return.
void err_msg(const char *fmt, ...) {
    va_list ap;        
    va_start(ap, fmt); 

    err_doit(0, fmt, ap);
    
    va_end(ap);        
    
    return;
}

// Fatal error unrelated to a system call. Print a message and terminate.
void err_quit(const char *fmt, ...) {
    va_list ap;        
    va_start(ap, fmt); 

    err_doit(0, fmt, ap);
    
    va_end(ap);
            
    exit(1);
}