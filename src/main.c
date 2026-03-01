#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "proc.h"
#include "display.h"

static volatile sig_atomic_t stop = 0;

static void handle_signal(int sig) {
    (void)sig;
    stop = 1;
}

static void cleanup(void) {
    printf("\033[0m\033[?25h");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    char *filter = NULL;
    if (argc > 1) {
        filter = argv[1];
    }

    struct sigaction sa = {0};
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    atexit(cleanup);

    Process sample_a[MAX_PROCESSES];
    Process sample_b[MAX_PROCESSES];

    while (!stop) {
        CpuSample cpu_a = read_cpu_sample();
        int count_a = read_processes(sample_a);

        sleep(1);

        CpuSample cpu_b = read_cpu_sample();
        int count_b = read_processes(sample_b);

        calculate_cpu(sample_a, count_a, sample_b, count_b, cpu_a, cpu_b);
        sort_by_cpu(sample_b, count_b);
        display_processes(sample_b, count_b, filter);
    }

    return 0;
}
