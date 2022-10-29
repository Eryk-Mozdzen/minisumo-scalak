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

#define FSM_STATE_MAX_NUM	10
#define FSM_EVENT_MAX_NUM	4

typedef void (*__fsm_behavior_t)();
typedef uint8_t (*__fsm_getter_t)();

struct __fsm_state;

/**
 * @brief event object
 */
struct __fsm_event {
	__fsm_getter_t get;				/*< function to check if event occurs, set NULL if no event is needed to switch state */
    struct __fsm_state *next_state;	/*< pointer to next state */
};

/**
 * @brief state object
 */
struct __fsm_state {
	uint8_t id;                						/*< user identifier of specific state, must be uniq */

	__fsm_behavior_t enter;      					/*< function called once, at the beginning of the state, after switch from previous one, set NULL if not needed */
	__fsm_behavior_t execute;    					/*< function called every time when the state is used, set NULL if not needed  */
	__fsm_behavior_t exit;       					/*< function called once, at the end of the state, before switch to the next one, set NULL if not needed  */

	struct __fsm_event events[FSM_EVENT_MAX_NUM];	/*< set of defined events for this state */
	uint8_t events_num;								/*< number of defined events */
};

/**
 * @brief state machine object structure
 */
typedef struct {
	struct __fsm_state *curr_state;					/*< pointer to current state */
    struct __fsm_state states[FSM_STATE_MAX_NUM];	/*< indexed set of defined states */
	uint8_t states_num;								/*< number of defined states */
} fsm_t;

void fsm_init(fsm_t *);

void fsm_define_state(fsm_t *, uint8_t, __fsm_behavior_t, __fsm_behavior_t, __fsm_behavior_t);
void fsm_define_transition(fsm_t *, uint8_t, uint8_t, __fsm_getter_t);

void fsm_start(fsm_t *, uint8_t);
void fsm_update(fsm_t *);
void fsm_execute(fsm_t *);

#endif
