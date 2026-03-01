#ifndef DISPLAY_H
#define DISPLAY_H

#include "proc.h"

void display_processes(Process *processes, int count);
void sort_by_cpu(Process *processes, int count);

#endif