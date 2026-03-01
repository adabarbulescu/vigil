#include <stdio.h>
#include <unistd.h>
#include "proc.h"

int main(void) {
    Process sample_a[MAX_PROCESSES];
    Process sample_b[MAX_PROCESSES];

    // First sample
    CpuSample cpu_a = read_cpu_sample();
    int count = read_processes(sample_a);

    // Wait 1 second
    sleep(1);

    // Second sample
    CpuSample cpu_b = read_cpu_sample();
    int count_b = read_processes(sample_b);

    // Calculate CPU percentages
    calculate_cpu(sample_a, count, sample_b, count_b, cpu_a, cpu_b);

    printf("%-10s %-25s %10s %10s\n", "PID", "NAME", "MEM(KB)", "CPU%");
    printf("%-10s %-25s %10s %10s\n", "----------", "-------------------------", "----------", "----------");

    for (int i = 0; i < count; i++) {
        
        printf("%-10d %-25s %10ld %9.1f%%\n",
            sample_b[i].pid,
            sample_b[i].name,
            sample_b[i].memory_kb,
            sample_b[i].cpu_percent);
        
    }

    printf("\nTotal processes: %d\n", count);
    return 0;
}