#include "scheduler.h"

typedef enum {
	TASK_STATE_READY,
	TASK_STATE_RUNNING,
	TASK_STATE_BLOCKED
} task_state_t;

typedef struct {
	uint8_t id;
	uint16_t delay;
	uint16_t period;
	task_t task;
	task_state_t state;
} tcb_t;

static tcb_t task_list[SCHEDULER_TASK_MAX_NUM];

void scheduler_add_task(const uint8_t id, const task_t task, const uint16_t period) {

    for(uint8_t i=0; i<SCHEDULER_TASK_MAX_NUM; i++) {

        if(!task_list[i].task) {

            task_list[i].id = id;
            task_list[i].task = task;
            task_list[i].delay = period;
            task_list[i].period = period;
            task_list[i].state = TASK_STATE_BLOCKED;

            return;
        }
    }
}

void scheduler_delete_task(const uint8_t id) {
	for(uint8_t i=0; i<SCHEDULER_TASK_MAX_NUM; i++) {
		if(task_list[i].id==id) {

			task_list[i].task = NULL;

			return;
		}
	}
}

void scheduler_start() {

	while(1) {

		for(uint8_t i=0; i<SCHEDULER_TASK_MAX_NUM; i++) {

			if(task_list[i].state==TASK_STATE_READY && task_list[i].task) {

				task_list[i].state = TASK_STATE_RUNNING;
				(*task_list[i].task)();
				task_list[i].delay = task_list[i].period;
				task_list[i].state = TASK_STATE_BLOCKED;
			}
		}
	}
}

void scheduler_tick() {

	for(uint8_t i=0; i<SCHEDULER_TASK_MAX_NUM; i++) {

		if(task_list[i].state==TASK_STATE_BLOCKED) {
			if(task_list[i].delay) {
				task_list[i].delay--;
			} else {
				task_list[i].state = TASK_STATE_READY;
			}
		}
	}
}