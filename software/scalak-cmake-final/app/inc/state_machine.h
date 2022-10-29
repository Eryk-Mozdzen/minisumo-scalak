/**
 * @file finite_state_machine.h
 * @author Eryk Możdżeń
 * @date 2021-12-24
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define FSM_STATE_MAX_NUM		10
#define FSM_TRANSITION_MAX_NUM	4

typedef void (*__fsm_behavior_t)();
typedef uint8_t (*__fsm_getter_t)();

/**
 * @brief event object
 */
typedef struct {
	__fsm_getter_t get;			/*< function to check if event occurs, set NULL if no event is needed to switch state */

    uint8_t next_index;			/*< index of next state */
} __fsm_event_t;

/**
 * @brief state object
 */
typedef struct {
	uint8_t id;                						/*< user identifier of specific state, must be uniq */

	__fsm_behavior_t enter;      					/*< function called once, at the beginning of the state, after switch from previous one, set NULL if not needed */
	__fsm_behavior_t execute;    					/*< function called every time when the state is used, set NULL if not needed  */
	__fsm_behavior_t exit;       					/*< function called once, at the end of the state, before switch to the next one, set NULL if not needed  */

	__fsm_event_t events[FSM_TRANSITION_MAX_NUM];	/*< set of defined events for this state */
    uint8_t events_num;        						/*< number of defined events for this state */
} __fsm_state_t;

/**
 * @brief state machine object structure
 */
typedef struct {
    uint8_t curr_state_index;  					/*< index of current state */

    __fsm_state_t states[FSM_STATE_MAX_NUM];	/*< indexed set of defined states */
    uint8_t states_num;        					/*< number of defined states */
} fsm_t;

void fsm_init(fsm_t *);

uint8_t fsm_define_state(fsm_t *, uint8_t, __fsm_behavior_t, __fsm_behavior_t, __fsm_behavior_t);
uint8_t fsm_define_transition(fsm_t *, uint8_t, uint8_t, __fsm_getter_t);

uint8_t fsm_start(fsm_t *, uint8_t);
uint8_t fsm_update(fsm_t *);
uint8_t fsm_execute(fsm_t *);

#endif
