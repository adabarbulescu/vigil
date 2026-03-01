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

int read_processes(Process *processes);

#endif