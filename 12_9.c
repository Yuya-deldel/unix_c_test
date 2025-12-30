#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "_header.h"

#ifndef MAP_FILE
#define MAP_FILE 0
#endif 

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH     // -rw-r--r--

int main(int argc, char *argv[]) {
    int fdin, fdout;
    char *src, *dst;
    struct stat statbuf;

    if (argc != 3) err_quit("usage: ./a.out <fromfile> <tofile>");

    // file open
    if ((fdin = open(argv[1], O_RDONLY)) < 0) err_sys("can't open %s for reading", argv[1]);
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) err_sys("can't create %s for writing", argv[2]);
    // set size of output file
    if (fstat(fdin, &statbuf) < 0) err_sys("fstat error");
    if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) err_sys("lseek error");  // offset を EOF の前に設定
    if (write(fdout, "", 1) != 1) err_sys("write error");       // EOF を上書き
    // ファイルをメモリにマップ
    if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fdin, 0)) == (__caddr_t) -1) err_sys("mmap error for input");
    if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fdout, 0)) == (__caddr_t) -1) err_sys("mmap error for output");
    // copy
    memcpy(dst, src, statbuf.st_size);
    
    exit(0);
}