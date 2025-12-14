#include "_header.h" 

int main(void) {
    int c;
    while ((c = getc(stdin)) != EOF) {  // EOF になるまで stdin から一文字ずつ読み込む
        if (putc(c, stdout) == EOF) err_sys("output error");    // stdout に一文字ずつ出力
    }

    // getc はエラーと EOF を区別しない
    if (ferror(stdin)) err_sys("input error");  // EOF でなくエラーだった場合

    exit(0);
}