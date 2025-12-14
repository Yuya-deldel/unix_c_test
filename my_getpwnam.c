#include <sys/types.h>
#include <pwd.h>
#include <stddef.h>
#include <string.h>

struct passwd * my_getpwnam(const char *name) {
    struct passwd *ptr;
    
    setpwent();     // getpwent が呼び出されていても、確実にファイルを巻き戻す
    while ((ptr = getpwent()) != NULL) {    // password ファイルの pointer 取得
        if (strcmp(name, ptr->pw_name) == 0) break;
    }
    endpwent();     // password ファイルをクローズ (忘れない)

    return ptr;
}