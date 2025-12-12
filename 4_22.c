#include "_header.h" 

int main(void) {
    // カレントディレクトリ移動
    if (chdir("/home/deltagraviton/workspace") < 0) err_sys("chdir failed");
    
    size_t size;
    char *ptr = path_alloc(&size);
    if (getcwd(ptr, size) == NULL) err_sys("getcwd failed");
    printf("cwd = %s\n", ptr);

    exit(0);
}