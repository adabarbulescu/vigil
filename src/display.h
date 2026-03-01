#ifndef DISPLAY_H
#define DISPLAY_H

#include "proc.h"

void display_processes(Process *processes, int count, const char *filter);
void sort_by_cpu(Process *processes, int count);

#endif
