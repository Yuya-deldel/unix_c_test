#include "_header.h" 

int main(void) {
    char name[L_tmpnam], line[MAXLINE];
    FILE *fp;

    printf("%s\n", tmpnam(NULL));       // first temp name

    tmpnam(name);       // second temp name
    printf("%s\n", name);

    if ((fp = tmpfile()) == NULL) err_sys("tmpfile error");     // create temp file 
    fputs("one line of output\n", fp);      // write to temp file 
    rewind(fp);     // read it back (ストリームの先頭に移動)
    if (fgets(line, sizeof(line), fp) == NULL) err_sys("fgets error");
    fputs(line, stdout);

    exit(0);
}