/**
 * @file finite_state_machine.c
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#include "finite_state_machine.h"

/**
 * @brief functions only for internal use
 */
static uint8_t get_state_index(fsm_t *, uint8_t, uint8_t *);

/**
 * @brief Initialize fields in state machine structure
 * @param st pointer to state machine
 * @param buffer memory buffer for user data
 */
void fsm_init(fsm_t *st) {
    // init fields
    st->curr_state_index = 0;
    st->states_num = 0;
}

/**
 * @brief Add state definition to state machine
 * @param st pointer to state machine
 * @param id state identifier
 * @param enter state enter function
 * @param execute state execute function
 * @param exit state exit function
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t fsm_define_state(fsm_t *st, uint8_t id, __fsm_behavior_t enter, __fsm_behavior_t execute, __fsm_behavior_t exit) {
    // check if already exist
    if(!get_state_index(st, id, NULL))
        return EXIT_FAILURE;

    st->states_num++;

    // write init values
    st->states[st->states_num-1].id = id;
    st->states[st->states_num-1].enter = enter;
    st->states[st->states_num-1].execute = execute;
    st->states[st->states_num-1].exit = exit;
    st->states[st->states_num-1].events_num = 0;
    
    return EXIT_SUCCESS;
}

/**
 * @brief Set transition between two states caused by presents of specific event
 * @param st pointer to state machine
 * @param curr_id identifier of the state when event occurred
 * @param next_id identifier of the state that should be set after event occurred
 * @param get event getter function
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t fsm_define_transition(fsm_t *st, uint8_t curr_id, uint8_t next_id, __fsm_getter_t get) {
	if(curr_id==next_id)
    	return EXIT_FAILURE;

    // get internal indexes
    uint8_t curr_index = 0;
    uint8_t next_index = 0;
    if(get_state_index(st, curr_id, &curr_index))
        return EXIT_FAILURE;
    if(get_state_index(st, next_id, &next_index))
        return EXIT_FAILURE;

    // allocate memory
    st->states[curr_index].events_num++;

    // write data
    st->states[curr_index].events[st->states[curr_index].events_num-1].get = get;
    st->states[curr_index].events[st->states[curr_index].events_num-1].next_index = next_index;

    return EXIT_SUCCESS;
}

/**
 * @brief Set inital state for state machine, call enter function if exist
 * @param st pointer to state machine
 * @param initial_id identifier of state that should be at the begin of the program, must be defined before
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t fsm_start(fsm_t *st, uint8_t initial_id) {
    if(get_state_index(st, initial_id, &st->curr_state_index))
        return EXIT_FAILURE;

    // call enter function for initial state
    if(st->states[st->curr_state_index].enter)
        st->states[st->curr_state_index].enter();

    return EXIT_SUCCESS;
}

/**
 * @brief Check if any event occurred, change state if it's required
 * @param st pointer to state machine
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t fsm_update(fsm_t *st) {
    if(!st->states_num)
        return EXIT_FAILURE;

    // check if any event defined for current state occurs
    __fsm_event_t *event = NULL;
    for(uint8_t i=0; i<st->states[st->curr_state_index].events_num; i++) {
    	// check if event occurs
    	// if event getter is NULL, no event is needed to switch state
    	uint8_t occur = 1;
    	if(st->states[st->curr_state_index].events[i].get)
    		occur = st->states[st->curr_state_index].events[i].get();

		if(occur) {
			event = &st->states[st->curr_state_index].events[i];
			break;
		}
    }

    if(event) {
		// call exit function for current state if exist
		if(st->states[st->curr_state_index].exit)
			st->states[st->curr_state_index].exit();

		// call enter function for next state if exist
		if(st->states[event->next_index].enter)
			st->states[event->next_index].enter();

		// change current state
		st->curr_state_index = event->next_index;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Call execute function for current state
 * @param st pointer to state machine
 * @return status of operation, 0 if success, 1 if error
 */
uint8_t fsm_execute(fsm_t *st) {
	if(!st->states_num)
		return EXIT_FAILURE;

	// call execute function for current state if exist
	if(st->states[st->curr_state_index].execute)
		st->states[st->curr_state_index].execute();

	return EXIT_SUCCESS;
}

/**
 * @brief Search for internal index of specific state.
 * @param st pointer to state machine
 * @param state_id identifier of the state
 * @param state_index internal index of this state, unknown if state does not exist
 * @return status of operation, 0 if success, 1 if state not defined
 */
uint8_t get_state_index(fsm_t *st, uint8_t state_id, uint8_t *state_index) {
    for(uint8_t i=0; i<st->states_num; i++) {
        if(st->states[i].id==state_id) {
            if(state_index)
                *state_index = i;
            return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}
