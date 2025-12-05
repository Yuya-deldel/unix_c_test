#include "_header.h"

int main(void) {
    // getpid(): process id を取得する
    printf("hello world from process ID %d\n", getpid());
    exit(0);
}