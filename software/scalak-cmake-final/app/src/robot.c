#include "robot.h"
#include "rc5.h"
#include "led.h"
#include "motors.h"
#include "periph.h"
#include "state_machine.h"
#include "scheduler.h"

#define ROBOT_EEPROM_ADDRESS_START		0x00
#define ROBOT_EEPROM_ADDRESS_STOP		0x01
#define ROBOT_EEPROM_ADDRESS_STATE		0x02

#define ROBOT_READY_LED_COUNTER_MAX		((uint16_t)240)

typedef enum {
	ROBOT_STATE_READY = 0xFF,	// eeprom reset value
	ROBOT_STATE_PROGRAM = 0x10,
	ROBOT_STATE_RUN = 0x20,
	ROBOT_STATE_STOP1 = 0x30,
	ROBOT_STATE_STOP2 = 0x40
} robot_state_t;

#define ARRAY_SIZE	8

static const uint8_t array[ARRAY_SIZE][3] = {
	{  0,   0, 255},
	{127,   0, 255},
	{255,   0, 255},
	{127,   0, 255},
	{  0,   0, 255},
	{  0, 127, 255},
	{  0, 255, 255},
	{  0, 127, 255}
};

static fsm_t fsm;
static int16_t counter;
static uint16_t limit;

static uint8_t eeprom_start_cmd;
static uint8_t eeprom_stop_cmd;

static uint8_t flag_program_cmd;
static uint8_t flag_start_cmd;
static uint8_t flag_stop_cmd;

static uint8_t flag_last_enemy_dir;

void loop_task() {
	rc5_message_t rc5_msg;

	if(rc5_get_message(&rc5_msg)) {
		if(RC5_MESSAGE_ADDRESS(rc5_msg)==0x0B) {
			// programming command
			flag_program_cmd = 1;

			eeprom_start_cmd = (RC5_MESSAGE_COMMAND(rc5_msg) & 0xFE) + 1;
			eeprom_stop_cmd = RC5_MESSAGE_COMMAND(rc5_msg) & 0xFE;

			eeprom_write(ROBOT_EEPROM_ADDRESS_START, eeprom_start_cmd);
			eeprom_write(ROBOT_EEPROM_ADDRESS_STOP, eeprom_stop_cmd);
		} else if(RC5_MESSAGE_COMMAND(rc5_msg)==eeprom_start_cmd) {
			// start command
			flag_start_cmd = 1;

		} else if(RC5_MESSAGE_COMMAND(rc5_msg)==eeprom_stop_cmd) {
			// stop command
			flag_stop_cmd = 1;

		}
	}

	fsm_update(&fsm);
	fsm_execute(&fsm);

	flag_program_cmd = 0;
	flag_start_cmd = 0;
	flag_stop_cmd = 0;
}

static uint8_t get_program() {
	return flag_program_cmd;
}

static uint8_t get_timeout() {
	return counter<=0;
}

static uint8_t get_start() {
	return flag_start_cmd;
}

static uint8_t get_stop() {
	return flag_stop_cmd;
}

static void ready_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_READY);

	motors_set(0, 0);

	counter = 0;
	limit = 20;
}

static void ready_execute() {

	if(scheduler_tick_count%2)
		return;
	
	const uint8_t (*color1)[3] = &array[((ARRAY_SIZE*counter)/ROBOT_READY_LED_COUNTER_MAX)%ARRAY_SIZE];
	const uint8_t (*color2)[3] = &array[(((ARRAY_SIZE*counter)/ROBOT_READY_LED_COUNTER_MAX) + 1)%ARRAY_SIZE];

	const uint16_t fraq = (ARRAY_SIZE*counter)%ROBOT_READY_LED_COUNTER_MAX;

	const uint8_t r = (((uint16_t)(*color2)[0])*fraq + ((uint16_t)(*color1)[0])*(ROBOT_READY_LED_COUNTER_MAX - fraq))/ROBOT_READY_LED_COUNTER_MAX;
	const uint8_t g = (((uint16_t)(*color2)[1])*fraq + ((uint16_t)(*color1)[1])*(ROBOT_READY_LED_COUNTER_MAX - fraq))/ROBOT_READY_LED_COUNTER_MAX;
	const uint8_t b = (((uint16_t)(*color2)[2])*fraq + ((uint16_t)(*color1)[2])*(ROBOT_READY_LED_COUNTER_MAX - fraq))/ROBOT_READY_LED_COUNTER_MAX;
	
	led_set(r/4, g/4, b/4);

	counter++;
	counter %=ROBOT_READY_LED_COUNTER_MAX;
}

static void program_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_READY);

	motors_set(0, 0);

	counter = 512/TICK_MS;
}

static void program_execute() {

	if(counter<128/TICK_MS) {
		led_set(0, 0, 0);
	} else if(counter<256/TICK_MS) {
		led_set(255, 255, 0);
	} else if(counter<384/TICK_MS) {
		led_set(0, 0, 0);
	} else {
		led_set(255, 255, 0);
	}

	counter--;
}

static void run_enter() {
	led_set(0, 255, 0);

	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_RUN);

	flag_last_enemy_dir = switch_get();
}

static void run_execute() {

	if(limit<255)
		limit++;

	motors_limit(limit);

	uint8_t prox[5];
	prox_get(prox);

	const int8_t weights[5] = {-90, -45, 0, 45, 90};
	int16_t sum = 0;
	int8_t count = 0;

	for(uint8_t i=0; i<5; i++) {
		if(prox[i]) {
			sum +=weights[i];
			count++;
		}
	}

	if(!count) {
		// enemy not spotted

		if(flag_last_enemy_dir) {
			motors_set(128, 0);
		} else {
			motors_set(0, 128);
		}

		return;
	}

	const int16_t dir = sum/count;

	flag_last_enemy_dir = dir>0;
	
	motors_set(
		255 + 4*dir,
		255 - 4*dir
	);
}

static void stop1_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_STOP1);

	motors_set(0, 0);

	led_set(255, 32, 0);

	counter = 1000/TICK_MS;
}

static void stop1_execute() {
	counter--;
}

static void stop2_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_READY);

	motors_set(0, 0);

	led_set(255, 0, 0);
}

void robot_init() {

	fsm_add_state(&fsm, ROBOT_STATE_READY,		ready_enter,	ready_execute,		NULL);
	fsm_add_state(&fsm, ROBOT_STATE_PROGRAM,	program_enter,	program_execute,	NULL);
	fsm_add_state(&fsm, ROBOT_STATE_RUN,		run_enter,		run_execute,		NULL);
	fsm_add_state(&fsm, ROBOT_STATE_STOP1,		stop1_enter,	stop1_execute,		NULL);
	fsm_add_state(&fsm, ROBOT_STATE_STOP2,		stop2_enter,	NULL,				NULL);

	fsm_add_transition(&fsm, ROBOT_STATE_READY,		ROBOT_STATE_PROGRAM,	get_program);
	fsm_add_transition(&fsm, ROBOT_STATE_PROGRAM,	ROBOT_STATE_READY,		get_timeout);
	fsm_add_transition(&fsm, ROBOT_STATE_READY,		ROBOT_STATE_RUN,		get_start);
	fsm_add_transition(&fsm, ROBOT_STATE_RUN,		ROBOT_STATE_PROGRAM,	get_program);
	fsm_add_transition(&fsm, ROBOT_STATE_RUN,		ROBOT_STATE_STOP1,		get_stop);
	fsm_add_transition(&fsm, ROBOT_STATE_STOP1,		ROBOT_STATE_STOP2,		get_timeout);

	eeprom_start_cmd = eeprom_read(ROBOT_EEPROM_ADDRESS_START);
	eeprom_stop_cmd = eeprom_read(ROBOT_EEPROM_ADDRESS_STOP);

	fsm_start(&fsm, eeprom_read(ROBOT_EEPROM_ADDRESS_STATE));
}
