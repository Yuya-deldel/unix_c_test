#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include "_header.h"

typedef int Myfunc(const char *, const struct stat *, int);
static int do_path(Myfunc *);

#define FTW_F 1     // file other than directory 
#define FTW_D 2     // directory 
#define FTW_DNR 3   // directory that can't be read 
#define FTW_NS 4    // file that we can't stat

static char *fullpath;
static size_t pathlen;
// user が与えた関数を呼び出しながらファイル階層を辿る
static int myftw(char *pathname, Myfunc *func) {
    fullpath = path_alloc(&pathlen);

    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) err_sys("realloc failed");
    }

    strcpy(fullpath, pathname);
    return do_path(func);
}

static int do_path(Myfunc *func) {
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;

    if (lstat(fullpath, &statbuf) < 0) {        // symbolic link を辿らず、ファイルの情報を取得
        return func(fullpath, &statbuf, FTW_NS);    // stat error
    } else if (S_ISDIR(statbuf.st_mode) == 0) {     // not a directory
        return func(fullpath, &statbuf, FTW_F);
    } 

    // directory case
    // まず "fullpath" directory 自体について func を実行する
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0 ) return ret;

    int n = strlen(fullpath);
    if (n + _PC_NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) err_sys("realloc failed");
    }
    
    fullpath[n++] = '/';
    fullpath[n] = 0;

    // 新たな "fullpath" directory を読み込む
    if ((dp = opendir(fullpath)) == NULL) {     
        return func(fullpath, &statbuf, FTW_DNR);   // can't read directory
    }
    while ((dirp = readdir(dp)) != NULL) {      // 中身を一つずつ check
        // ".", ".." は無視
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..")) continue;
        
        strcpy(&fullpath[n], dirp->d_name);      // .../fullpath/ のあとに d_name を追加

        // 再帰的に実行
        if ((ret = do_path(func)) != 0) break;    // error
    }
    fullpath[n-1] = 0;

    if (closedir(dp) < 0) err_ret("can't close directory %s", fullpath);
    
    return ret;
}

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock;
// ファイルの種類を数える (Myfunc の例)
static int myfunc(const char *pathname, const struct stat *statptr, int type) {
    switch (type) {
        case FTW_F:
            switch (statptr->st_mode & __S_IFMT) {      // ファイル識別用 bit mask
                case __S_IFREG: nreg++; break;
                case __S_IFBLK: nblk++; break;
                case __S_IFCHR: nchr++; break;
                case __S_IFIFO: nfifo++; break;
                case __S_IFLNK: nslink++; break;
                case __S_IFSOCK: nsock++; break;
                case __S_IFDIR: err_dump("__S_IFDIR for %s", pathname); // directory should have type FTW_D
            }
            break;

        case FTW_D: ndir++; break;

        case FTW_DNR: 
            err_ret("can't read directory %s", pathname); 
            break;
        case FTW_NS: 
            err_ret("stat error for %s", pathname); 
            break;
    
    default:
        err_dump("unknown type %d for pathname %s", type, pathname);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) err_quit("usage: ftw <starting-pathname>");

    int ret = myftw(argv[1], myfunc);

    int ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) ntot = 1;    // 0 除算を避ける

    printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    exit(ret);
}