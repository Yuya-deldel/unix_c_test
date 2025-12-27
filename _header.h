#ifndef _HEADER
#define _HEADER

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

#define min(a, b)   ((a) < (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))

// error_lib.c
void err_dump(const char *, ...);   // syscall に関連する; abort 終了
void err_msg(const char *, ...);    // syscall に関連しない; 通常終了
void err_quit(const char *, ...);   // syscall に関連しない; エラー終了
void err_ret(const char *, ...);    // syscall に関連する; 通常終了
void err_sys(const char *, ...);    // syscall に関連する; エラー終了

// posix_lib.c
char *path_alloc(size_t *);    // path_name 用のメモリ領域を確保
int open_max(void);         // プロセスあたりのオープン可能ファイルの最大数 (OPEN_MAX) を取得
#endif

// process_lib.c 
void pr_exit(int);      // 終了状態を表示
void pr_mask(const char *);

// fd_flag_lib.c
void set_fl(int, int);
void clr_fl(int, int);

// signal
typedef void Sigfunc(int);

// tell_wait_lib.c
void tell_wait(void);
void tell_parent(pid_t);
void wait_parent(void);
void tell_child(pid_t);
void wait_child(void);

// my_terms_lib.c
int isatty(int);
char *ttyname(int);

// tty_lib.c 
int tty_cbreak(int);
int tty_raw(int);
int tty_reset(int);
void tty_atexit(void);
struct termios *tty_termios(void);

// lock_lib.c 
int lock_leg(int, int, int, off_t, int, off_t);
#define read_lock(fd, offset, whence, len)      lock_leg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define readw_lock(fd, offset, whence, len)     lock_leg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define write_lock(fd, offset, whence, len)     lock_leg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define writew_lock(fd, offset, whence, len)    lock_leg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define un_lock(fd, offset, whence, len)        lock_leg(fd, F_SETLK, F_UNLCK, offset, whence, len)

pid_t lock_test(int, int, off_t, int, off_t);
#define is_readlock(fd, offset, whence, len)    lock_test(fd, F_RDLCK, offset, whence, len)
#define is_writelock(fd, offset, whence, len)   lock_test(fd, F_WRLCK, offset, whence, len)