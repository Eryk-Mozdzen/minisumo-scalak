#include "state_machine.h"

static struct __fsm_state * get_state_ptr(fsm_t *st, uint8_t id) {
    for(uint8_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==id) {
			return &st->states[i];
        }
    }
    
    return NULL;
}

void fsm_add_state(fsm_t *st, uint8_t id, __fsm_behavior_t enter, __fsm_behavior_t execute, __fsm_behavior_t exit) {

	if(st->states_num>=FSM_STATE_MAX_NUM)
		return;

    st->states[st->states_num].id = id;
    st->states[st->states_num].enter = enter;
    st->states[st->states_num].execute = execute;
    st->states[st->states_num].exit = exit;
	st->states[st->states_num].events_num = 0;
	st->states_num++;
}

void fsm_add_transition(fsm_t *st, uint8_t curr_id, uint8_t next_id, __fsm_getter_t get) {

	struct __fsm_state *curr_state = get_state_ptr(st, curr_id);
	struct __fsm_state *next_state = get_state_ptr(st, next_id);

	if(!curr_state || !next_state)
		return;

	if(curr_state->events_num>=FSM_EVENT_MAX_NUM)
		return;

    curr_state->events[curr_state->events_num].get = get;
    curr_state->events[curr_state->events_num].next_state = next_state;
	curr_state->events_num++;
}

void fsm_start(fsm_t *st, uint8_t initial_id) {

	struct __fsm_state *initial_state = get_state_ptr(st, initial_id);

    if(!initial_state)
        return;

	st->curr_state = initial_state;

    if(st->curr_state->enter)
        st->curr_state->enter();
}

void fsm_update(fsm_t *st) {

	if(!st->curr_state)
		return;

	const struct __fsm_event *event = NULL;

    for(uint8_t i=0; i<st->curr_state->events_num; i++) {

    	if(st->curr_state->events[i].get) {
    		if(st->curr_state->events[i].get())
				event = &st->curr_state->events[i];
		} else {
			event = &st->curr_state->events[i];
		}
    }

	if(!event)
		return;

	if(st->curr_state->exit)
		st->curr_state->exit();

	st->curr_state = event->next_state;

	if(st->curr_state->enter)
		st->curr_state->enter();
}

void fsm_execute(fsm_t *st) {

	if(!st->curr_state)
		return;

	if(st->curr_state->execute)
		st->curr_state->execute();
}
