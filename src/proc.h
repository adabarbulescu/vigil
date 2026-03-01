#ifndef PROC_H
#define PROC_H

#define MAX_NAME_LEN 256
#define MAX_PROCESSES 1024

typedef struct {
    int pid;
    char name[MAX_NAME_LEN];
    long memory_kb;
    double cpu_percent;
    unsigned long utime;
    unsigned long stime;
} Process;

typedef struct {
    unsigned long total;
    unsigned long idle;
} CpuSample;

int read_processes(Process *processes);
CpuSample read_cpu_sample(void);
void calculate_cpu(Process *a, int count_a, Process *b, int count_b, CpuSample sa, CpuSample sb);

#endif