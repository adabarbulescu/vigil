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

int read_processes(Process *processes) {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) return 0;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(proc_dir)) != NULL && count < MAX_PROCESSES) {
        if (!is_pid_dir(entry->d_name)) continue;

        int pid = atoi(entry->d_name);
        char path[64];

        // Read process name
        snprintf(path, sizeof(path), "/proc/%d/comm", pid);
        FILE *f = fopen(path, "r");
        if (!f) continue;

        char name[MAX_NAME_LEN];
        if (fgets(name, sizeof(name), f) == NULL) {
            fclose(f);
            continue;
        }
        fclose(f);

        // Strip newline
        name[strcspn(name, "\n")] = '\0';

        // Read memory from /proc/[pid]/status
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
        processes[count].memory_kb = memory_kb;
        processes[count].cpu_percent = 0.0;
        processes[count].utime = 0;
        processes[count].stime = 0;
        count++;
    }

    closedir(proc_dir);
    return count;
}