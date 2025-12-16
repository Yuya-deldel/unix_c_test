#include "_header.h"

static void my_exit_1(void) {
    printf("first exit handler\n");
}

static void my_exit_2(void) {
    printf("second exit handler\n");
}

int main(void) {
    if (atexit(my_exit_2) != 0) err_sys("can't register my_exit_2");
    if (atexit(my_exit_1) != 0) err_sys("can't register my_exit_1");
    if (atexit(my_exit_1) != 0) err_sys("can't register my_exit_1");

    printf("main is done\n");
    return(0);
}