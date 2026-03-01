#include <stdio.h>
#include "proc.h"

int main(void) {
    Process processes[MAX_PROCESSES];
    int count = read_processes(processes);

    printf("%-10s %-25s %10s\n", "PID", "NAME", "MEMORY(KB)");
    printf("%-10s %-25s %10s\n", "----------", "-------------------------", "----------");

    for (int i = 0; i < count; i++) {
        printf("%-10d %-25s %10ld\n",
            processes[i].pid,
            processes[i].name,
            processes[i].memory_kb);
    }

    printf("\nTotal processes: %d\n", count);
    return 0;
}