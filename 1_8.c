#include "_header.h"

int main(void) {
    // uid: ユーザーID, gid: グループID
    printf("uid = %d, gid = %d\n", getuid(), getgid()); 
    exit(0);
}