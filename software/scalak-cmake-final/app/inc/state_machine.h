#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

#define FSM_STATE_MAX_NUM	10
#define FSM_EVENT_MAX_NUM	4

typedef void (*__fsm_behavior_t)();
typedef uint8_t (*__fsm_getter_t)();

struct __fsm_state;

struct __fsm_event {
	__fsm_getter_t get;
    struct __fsm_state *next_state;
};

struct __fsm_state {
	uint8_t id;

	__fsm_behavior_t enter;
	__fsm_behavior_t execute;
	__fsm_behavior_t exit;

	struct __fsm_event events[FSM_EVENT_MAX_NUM];
	uint8_t events_num;
};

typedef struct {
	struct __fsm_state *curr_state;
    struct __fsm_state states[FSM_STATE_MAX_NUM];
	uint8_t states_num;
} fsm_t;

void fsm_init(fsm_t *);
void fsm_define_state(fsm_t *, uint8_t, __fsm_behavior_t, __fsm_behavior_t, __fsm_behavior_t);
void fsm_define_transition(fsm_t *, uint8_t, uint8_t, __fsm_getter_t);

void fsm_start(fsm_t *, uint8_t);
void fsm_update(fsm_t *);
void fsm_execute(fsm_t *);

#endif
