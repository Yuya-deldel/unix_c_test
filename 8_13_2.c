#include <sys/types.h>
#include <sys/acct.h>
#include "_header.h"

#define ACCTFILE "/var/adm/pacct"

// comp_t -> unsigned long
static unsigned long compt2long(comp_t comptime) {
    unsigned long val = comptime & 0x1fff;      // 13-bit
    int exp = (comptime >> 13) & 7;             // 3-bit exponent 
    while (exp-- > 0) {
        val *= 8;
    }

    return val;
}

int main(void) {
    struct acct acdata;
    FILE *fp;

    if ((fp = fopen(ACCTFILE, "r")) == NULL) err_sys("can't open %s", ACCTFILE);

    while (fread(&acdata, sizeof(acdata), 1, fp) == 1) {    // バイナリ読み込み
        // printf();
    }

    if (ferror(fp)) err_sys("read error");  // read error か EOF か判別

    exit(0);
}