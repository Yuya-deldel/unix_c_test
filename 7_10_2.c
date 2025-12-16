#include <setjmp.h>
#include "_header.h"

static jmp_buf jmpbuffer;

static void f2(void) {
    longjmp(jmpbuffer, 1);
}

static void f1(int i, int j, int k) {
    printf("in f1(): count = %d, val = %d, sum = %d\n", i, j, k);
    f2();
}

int main(void) {
    int count = 2;
    register int val = 3;
    volatile int sum = 4;

    if (setjmp(jmpbuffer) != 0) {
        printf("after longjmp: count = %d, val = %d, sum = %d\n", count, val, sum);
        exit(0);
    }

    count = 97;
    val = 98;
    sum = 99;

    f1(count, val, sum);    // never returns
}