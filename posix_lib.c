#include <errno.h>
#include <limits.h>
#include "_header.h"

// PATH_MAX は判定不能の場合がある (古い情報?)
#ifdef PATH_MAX     // <limits.h> で定義されていればそれを用いる
static int pathmax = PATH_MAX;
#else 
static int pathmax = 0;
#endif 

#define PATH_MAX_GUESS 1024     // if PATH_MAX is indeterminate

// OPEN_MAX は判定不能の場合がある (古い情報?)
#ifdef OPEN_MAX     // <limits.h> で定義されていればそれを用いる
static int openmax = OPEN_MAX;
#else 
static int openmax = 0;
#endif

#define OPEN_MAX_GUESS 256       // if OPEN_MAX if indeterminate

// path 名用のメモリ領域を確保 (+ size を更新)
// PATH_MAX が定義されていなければ処理する
char *path_alloc(size_t *size) {   // nonnull の場合 size の値も更新する
    char *ptr; 
    if (pathmax == 0) {     // PATH_MAX が未設定
        errno = 0;
        // pathconf(path, _PC_PATH_MAX) -> path を current directory とする場合の相対path名の最大値
        // "/" は root directory
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {  // error case
            if (errno == 0) pathmax = PATH_MAX_GUESS;       // PATH_MAX が未定義の場合の妥協策
            else err_sys("pathconf error for _PC_PATH_MAX");
        } else {
            pathmax += 1;   // "/" 分を追加
        }
    }

    if ((ptr = malloc(pathmax + 1)) == NULL) err_sys("malloc error for pathname");
    if (size != NULL) *size = pathmax + 1;      // 終端記号分を追加

    return ptr;
}

// プロセスあたりのオープン可能ファイルの最大数 (OPEN_MAX) を取得
// 未定義ならば適切に処理
int open_max(void) {
    if (openmax == 0) {     // OPEN_MAX が未設定
        errno = 0;
        // sysconf: _SC_ で始まる実行時制限事項を取得
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {    // error case
            if (errno == 0) openmax = OPEN_MAX_GUESS;   // OPEN_MAX が未定義の場合の妥協策
            else err_sys("sysconf error for _SC_OPEN_MAX");
        }
    }

    return openmax;
}