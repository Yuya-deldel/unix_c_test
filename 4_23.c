#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "_header.h"

int main(int argc, char *argv[]) {
    struct stat buf;

    for (int i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        }
        printf("dev = %d / %d", major(buf.st_dev), minor(buf.st_dev));

        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            char *type = S_ISCHR(buf.st_mode) ? "character" : "block";
            printf("(%s) rdev = %d / %d", type, major(buf.st_rdev), minor(buf.st_rdev));
        }

        printf("\n");
    }
    exit(0);
}