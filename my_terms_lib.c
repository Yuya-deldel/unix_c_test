#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>

static char ctermid_name[L_ctermid];
char *ctermid(char *str) {
    if (str == NULL) str = ctermid_name;
    return (strcpy(str, "/dev/tty"));
}


#include <termios.h>

int isatty(int fd) {
    struct termios term;
    return (tcgetattr(fd, &term) != -1);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h> 

#define DEV "/dev/"     // device directory 
#define DEVLEN sizeof(DEV)-1

char *ttyname(int fd) {     // fd: ファイルデスクリプタ
    struct stat fdstat, devstat;
    DIR *dp;
    struct dirent *dirp;
    static char pathname[_POSIX_PATH_MAX + 1];
    char *rval = NULL;

    // error ならは NULL を返す
    if (isatty(fd) == 0) return NULL;
    if (fstat(fd, &fdstat) < 0) return NULL;    // ファイルデスクリプタの情報を fdstat に収める
    if (S_ISCHR(fdstat.st_mode) == 0) return NULL;      // キャラクターデバイスでない
    if ((dp = opendir(DEV)) == NULL) return NULL;       // デバイスディレクトリを開く
    
    strcpy(pathname, DEV);

    // デバイスディレクトリの中から、i-node 番号と装置番号が一致するものを探す
    while ((dirp = readdir(dp)) != NULL) {     
        if (dirp->d_ino != fdstat.st_ino) {     // i-node 番号
            continue;                           
        }
        // pathname のあとに内容の名前をコピー
        strncpy(pathname + DEVLEN, dirp->d_name, _POSIX_PATH_MAX - DEVLEN);
        if (stat(pathname, &devstat) < 0) {     // directory の内容の情報を得る
            continue;
        }

        if (devstat.st_ino == fdstat.st_ino && devstat.st_dev == fdstat.st_dev) {   // match
            rval = pathname;
            break;
        }
    }
    closedir(dp);
    return rval;
}