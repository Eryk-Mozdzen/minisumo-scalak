/**
 * @file finite_state_machine.c
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include "state_machine.h"

/**
 * @brief Search for pointer to specific state.
 * @param st pointer to state machine
 * @param id identifier of the state
 * @return pointer to state, NULL if not defined
 */
static struct __fsm_state * get_state_ptr(fsm_t *st, uint8_t id) {
    for(uint8_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==id) {
			return &st->states[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Initialize fields in state machine structure
 * @param st pointer to state machine
 */
void fsm_init(fsm_t *st) {
    // init fields
    st->curr_state = NULL;
	st->states_num = 0;
}

/**
 * @brief Add state definition to state machine
 * @param st pointer to state machine
 * @param id state identifier
 * @param enter state enter function
 * @param execute state execute function
 * @param exit state exit function
 */
void fsm_define_state(fsm_t *st, uint8_t id, __fsm_behavior_t enter, __fsm_behavior_t execute, __fsm_behavior_t exit) {

	if(st->states_num>=FSM_STATE_MAX_NUM)
		return;

    // write init values
    st->states[st->states_num].id = id;
    st->states[st->states_num].enter = enter;
    st->states[st->states_num].execute = execute;
    st->states[st->states_num].exit = exit;
	st->states[st->states_num].events_num = 0;

	st->states_num++;
}

/**
 * @brief Set transition between two states caused by presents of specific event
 * @param st pointer to state machine
 * @param curr_id identifier of the state when event occurred
 * @param next_id identifier of the state that should be set after event occurred
 * @param get event getter function
 */
void fsm_define_transition(fsm_t *st, uint8_t curr_id, uint8_t next_id, __fsm_getter_t get) {

    // get states
	struct __fsm_state *curr_state = get_state_ptr(st, curr_id);
	struct __fsm_state *next_state = get_state_ptr(st, next_id);
	if(!curr_state || !next_state)
		return;

    // write data
	if(curr_state->events_num>=FSM_EVENT_MAX_NUM)
		return;

    curr_state->events[curr_state->events_num].get = get;
    curr_state->events[curr_state->events_num].next_state = next_state;
	
	curr_state->events_num++;
}

/**
 * @brief Set inital state for state machine, call enter function if exist
 * @param st pointer to state machine
 * @param initial_id identifier of state that should be at the begin of the program, must be defined before
 */
void fsm_start(fsm_t *st, uint8_t initial_id) {

	struct __fsm_state *initial_state = get_state_ptr(st, initial_id);
    if(!initial_state)
        return;

	// set current state to initial state
	st->curr_state = initial_state;

    // call enter function for current state
    if(st->curr_state->enter)
        st->curr_state->enter();
}

/**
 * @brief Check if any event occurred, change state if it's required
 * @param st pointer to state machine
 */
void fsm_update(fsm_t *st) {

	if(!st->curr_state)
		return;

    // check if any event defined for current state occurs
	const struct __fsm_event * event = NULL;

    for(uint8_t i=0; i<st->curr_state->events_num; i++) {
    	// check if event occurs
    	// if event getter is NULL, no event is needed to switch state

    	if(st->curr_state->events[i].get) {
    		if(st->curr_state->events[i].get())
				event = &st->curr_state->events[i];
		} else {
			event = &st->curr_state->events[i];
		}
    }

	if(!event)
		return;

	// call exit function for current state if exist
	if(st->curr_state->exit)
		st->curr_state->exit();
	
	// change current state
	st->curr_state = event->next_state;

	// call enter function for next state if exist
	if(st->curr_state->enter)
		st->curr_state->enter();
}

/**
 * @brief Call execute function for current state
 * @param st pointer to state machine
 */
void fsm_execute(fsm_t *st) {
	if(!st->curr_state)
		return;

	// call execute function for current state if exist
	if(st->curr_state->execute)
		st->curr_state->execute();
}
