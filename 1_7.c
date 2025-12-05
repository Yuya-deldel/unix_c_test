#include <errno.h>
#include "_header.h"

int main(int argc, char *argv[]) {
    // EACCES に対応するエラーメッセージを標準エラー出力に出力
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES)); 
    // EACCES: Permission denied
    
    errno = ENOENT;     
    perror(argv[0]);     // 現在の errno の値に対応するエラーメッセージを標準エラー出力に出力
    // 引数に argv[0] をとるのは UNIX の慣習: パイプライン処理したときにどこでエラーしたかが分かる
    // ./a.out: No such file or directory

    exit(0);
}