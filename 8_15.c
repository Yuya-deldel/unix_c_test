#include <sys/times.h>
#include "_header.h"

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend) {
    static long clktck = 0;

    if (clktck == 0) {
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {  // fetch clock tps first time
            err_sys("sysconf error");
        }
    }

    fprintf(stderr, "       real: %7.2f\n", real / (double) clktck);
    fprintf(stderr, "       user: %7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
    fprintf(stderr, "        sys: %7.2f\n", (tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
    fprintf(stderr, " child user: %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
    fprintf(stderr, "  child sys: %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}

static void do_cmd(char *cmd) {
    struct tms tmsstart, tmsend;
    clock_t start, end;
    int status;

    fprintf(stderr, "\ncommand: %s\n", cmd);

    if ((start = times(&tmsstart)) == -1) err_sys("times error");       // 開始時刻
    if ((status = system(cmd)) < 0) err_sys("system() error");          // コマンド実行
    if ((end = times(&tmsend)) == -1) err_sys("times error");           // 終了時刻

    pr_times(end - start, &tmsstart, &tmsend);
    pr_exit(status);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        do_cmd(argv[i]);
    }

    exit(0);
}