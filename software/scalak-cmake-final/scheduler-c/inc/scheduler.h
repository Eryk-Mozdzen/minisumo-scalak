// https://sites.google.com/site/avrtutorials2/scheduler

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>

#define SCHEDULER_TASK_MAX_NUM	10

typedef void (*task_t)();

void scheduler_add_task(const task_t, const uint16_t);
void scheduler_start();
void scheduler_tick();

#endif