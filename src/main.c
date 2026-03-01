#include <stdio.h>
#include <unistd.h>
#include "proc.h"
#include "display.h"

int main(int argc, char *argv[]) {
    char *filter = NULL;
    if (argc > 1) {
        filter = argv[1];
    }
    Process sample_a[MAX_PROCESSES];
    Process sample_b[MAX_PROCESSES];

    while (1) {
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
