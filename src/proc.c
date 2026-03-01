#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "proc.h"

static int is_pid_dir(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] < '0' || name[i] > '9') return 0;
    }
    return 1;
}

static void read_cpu_ticks(int pid, unsigned long *utime, unsigned long *stime) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) { *utime = 0; *stime = 0; return; }

    char line[512];
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        *utime = 0; *stime = 0;
        return;
    }
    fclose(f);

    char *ptr = strrchr(line, ')');
    if (!ptr) { *utime = 0; *stime = 0; return; }
    ptr++;

    unsigned long ut, st;
    int matched = sscanf(ptr,
        " %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu",
        &ut, &st);

    if (matched == 2) {
        *utime = ut;
        *stime = st;
    } else {
        *utime = 0;
        *stime = 0;
    }
}

int read_processes(Process *processes) {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) return 0;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(proc_dir)) != NULL && count < MAX_PROCESSES) {
        if (!is_pid_dir(entry->d_name)) continue;

        int pid = atoi(entry->d_name);
        char path[64];

        snprintf(path, sizeof(path), "/proc/%d/comm", pid);
        FILE *f = fopen(path, "r");
        if (!f) continue;

        char name[MAX_NAME_LEN];
        if (fgets(name, sizeof(name), f) == NULL) {
            fclose(f);
            continue;
        }
        fclose(f);

        name[strcspn(name, "\n")] = '\0';

        snprintf(path, sizeof(path), "/proc/%d/status", pid);
        f = fopen(path, "r");
        if (!f) continue;

        long memory_kb = 0;
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line, "VmRSS: %ld", &memory_kb);
                break;
            }
        }
        fclose(f);

        processes[count].pid = pid;
        strncpy(processes[count].name, name, MAX_NAME_LEN - 1);
        processes[count].name[MAX_NAME_LEN - 1] = '\0';
        processes[count].memory_kb = memory_kb;
        read_cpu_ticks(pid, &processes[count].utime, &processes[count].stime);
        processes[count].cpu_percent = 0.0;
        count++;
    }

    closedir(proc_dir);
    return count;
}

CpuSample read_cpu_sample(void) {
    CpuSample sample = {0, 0};
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return sample;

    unsigned long user, nice, system, idle, iowait, irq, softirq;
    int matched = fscanf(f, "cpu %lu %lu %lu %lu %lu %lu %lu",
                         &user, &nice, &system, &idle, &iowait, &irq, &softirq);
    fclose(f);

    if (matched != 7) return sample;

    sample.idle = idle + iowait;
    sample.total = user + nice + system + idle + iowait + irq + softirq;
    return sample;
}

void calculate_cpu(Process *a, int count_a, Process *b, int count_b, CpuSample sa, CpuSample sb) {
    unsigned long total_delta = sb.total - sa.total;
    if (total_delta == 0) return;

    for (int i = 0; i < count_b; i++) {
        b[i].cpu_percent = 0.0;
        for (int j = 0; j < count_a; j++) {
            if (a[j].pid == b[i].pid) {
                unsigned long process_delta = (b[i].utime + b[i].stime) - (a[j].utime + a[j].stime);
                b[i].cpu_percent = ((double)process_delta / (double)total_delta) * 100.0;
                break;
            }
        }
    }
}
