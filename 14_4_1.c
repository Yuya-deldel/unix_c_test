#include "_header.h"

int main(void) {
    int n, int1, int2;
    char line[MAXLINE];

    while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0) {
        line[n] = 0;        // null terminate
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {      // parse 関数: 読み込みに成功した変数の数を返す
            sprintf(line, "%d\n", int1 + int2);
            n = strlen(line);
            if (write(STDOUT_FILENO, line, n) != n) err_sys("write error");
        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) err_sys("write error");
        }
    }
    exit(0);
}