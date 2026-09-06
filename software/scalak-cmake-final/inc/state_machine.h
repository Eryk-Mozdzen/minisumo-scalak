#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include <stddef.h>

#define FSM_STATE_MAX_NUM	10
#define FSM_EVENT_MAX_NUM	4

typedef uint8_t fsm_state_id_t;
typedef void (*fsm_behavior_t)();
typedef uint8_t (*fsm_getter_t)();

typedef struct fsm_state fsm_state_t;

typedef struct {
	fsm_getter_t get;
    fsm_state_t *next_state;
} fsm_event_t;

typedef struct fsm_state {
	fsm_state_id_t id;
	fsm_behavior_t enter;
	fsm_behavior_t execute;
	fsm_behavior_t exit;
	fsm_event_t events[FSM_EVENT_MAX_NUM];
	uint8_t events_num;
} fsm_state_t;

typedef struct {
	fsm_state_t *curr_state;
	fsm_state_t states[FSM_STATE_MAX_NUM];
	uint8_t states_num;
} fsm_t;

void fsm_add_state(fsm_t *, fsm_state_id_t, fsm_behavior_t, fsm_behavior_t, fsm_behavior_t);
void fsm_add_transition(fsm_t *, fsm_state_id_t, fsm_state_id_t, fsm_getter_t);

void fsm_start(fsm_t *, fsm_state_id_t);
void fsm_update(fsm_t *);
void fsm_execute(fsm_t *);

#endif
