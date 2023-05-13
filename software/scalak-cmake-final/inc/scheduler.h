// https://sites.google.com/site/avrtutorials2/scheduler

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>

extern volatile uint16_t scheduler_tick_count;

typedef void (*task_t)();

void scheduler_add_task(const task_t, const uint16_t);
void scheduler_start();

#endif
