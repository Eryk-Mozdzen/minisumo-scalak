#include "scheduler.h"

typedef struct {
	task_t task;
	uint16_t next;
	uint16_t period;
} tcb_t;

static tcb_t task_list[SCHEDULER_TASK_MAX_NUM];
static uint8_t task_num;
volatile uint16_t scheduler_tick_count;

void scheduler_add_task(const task_t task, const uint16_t period) {

	if(task_num>=SCHEDULER_TASK_MAX_NUM)
		return;

	task_list[task_num].task = task;
	task_list[task_num].next = scheduler_tick_count + period;
	task_list[task_num].period = period;

	task_num++;
}

void scheduler_start() {

	while(1) {

		for(uint8_t i=0; i<task_num; i++) {

			if(task_list[i].next<=scheduler_tick_count) {

				task_list[i].next = scheduler_tick_count + task_list[i].period;

				(*task_list[i].task)();
			}
		}
	}
}
