#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include <unistd.h>

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_HEADER  "\033[1;36m"   
#define COLOR_HIGH    "\033[1;31m"   
#define COLOR_MEDIUM  "\033[1;33m"   
#define COLOR_LOW     "\033[0;32m"   
#define COLOR_NORMAL  "\033[0m"      

static void clear_screen(void) {
    system("clear");
}

static const char *cpu_color(double cpu) {
    if (cpu > 10.0) return COLOR_HIGH;
    if (cpu > 2.0)  return COLOR_MEDIUM;
    if (cpu > 0.0)  return COLOR_LOW;
    return COLOR_NORMAL;
}

static int compare_cpu(const void *a, const void *b) {
    const Process *pa = (const Process *)a;
    const Process *pb = (const Process *)b;
    if (pb->cpu_percent > pa->cpu_percent) return 1;
    if (pb->cpu_percent < pa->cpu_percent) return -1;
    return 0;
}

void sort_by_cpu(Process *processes, int count) {
    qsort(processes, count, sizeof(Process), compare_cpu);
}

void display_processes(Process *processes, int count) {
    clear_screen();

    // Header
    printf("%s%-10s %-25s %10s %10s%s\n",
           COLOR_HEADER, "PID", "NAME", "MEM(KB)", "CPU%", COLOR_RESET);
    printf("%s%-10s %-25s %10s %10s%s\n",
           COLOR_HEADER,
           "----------", "-------------------------", "----------", "----------",
           COLOR_RESET);

    // Rows
    for (int i = 0; i < count; i++) {
        const char *color = cpu_color(processes[i].cpu_percent);
        printf("%s%-10d %-25s %10ld %9.1f%%%s\n",
               color,
               processes[i].pid,
               processes[i].name,
               processes[i].memory_kb,
               processes[i].cpu_percent,
               COLOR_RESET);
    }

    printf("\n%sTotal processes: %d%s\n", COLOR_HEADER, count, COLOR_RESET);
    fflush(stdout);
}
