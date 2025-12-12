#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include "_header.h"

typedef int Myfunc(const char *, const struct stat *, int);

// user が与えた関数を呼び出しながらファイル階層を辿る
static int myftw(char *, Myfunc *) {

}