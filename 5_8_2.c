#include "_header.h" 

int main(void) {
    char buf[MAXLINE];
    while ((fgets(buf, MAXLINE, stdin)) != NULL) {  // NULL はエラー
        if (fputs(buf, stdout) == EOF) err_sys("output error");     // stdout に一行ずつ出力
    }

    if (ferror(stdin)) err_sys("input error");  // EOF でなくエラーだった場合

    exit(0);        // 標準入出力ストリームをクローズしていない -> 代わりに exit
}                   // 自動的にフラッシュ + クローズ