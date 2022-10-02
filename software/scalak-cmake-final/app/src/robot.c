#include "robot.h"

typedef enum {
	ROBOT_STATE_READY = 0xFF,	// eeprom reset value
	ROBOT_STATE_PROGRAM = 0x10,
	ROBOT_STATE_RUN = 0x20,
	ROBOT_STATE_STOP1 = 0x30,
	ROBOT_STATE_STOP2 = 0x40
} robot_state_t;

const uint8_t array[12][3] = {
	{255,   0, 255},
	{255,   0, 127},
	{255,   0,   0},
	{255, 127,   0},
	{255, 255,   0},
	{127, 255,   0},
	{  0, 255,   0},
	{  0, 255, 127},
	{  0, 255, 255},
	{  0, 127, 255},
	{  0,   0, 255},
	{127,   0, 255}
};

static fsm_t fsm;
static rc5_message_t rc5_msg;
static int16_t ready_color_counter;

static uint8_t eeprom_start_cmd;
static uint8_t eeprom_stop_cmd;

static uint8_t flag_program_cmd;
static uint8_t flag_start_cmd;
static uint8_t flag_stop_cmd;

static void loop() {
	if(rc5_get_message(&rc5_msg)) {
		if(rc5_msg.command==eeprom_start_cmd) {
			// start command
			flag_start_cmd = 1;

		} else if(rc5_msg.command==eeprom_stop_cmd) {
			// stop command
			flag_stop_cmd = 1;

		} else if(rc5_msg.address==0x0B) {
			// programming command
			flag_program_cmd = 1;

			eeprom_start_cmd = (rc5_msg.command & 0xFE) + 1;
			eeprom_stop_cmd = rc5_msg.command & 0xFE;

			eeprom_write(ROBOT_EEPROM_ADDRESS_START, eeprom_start_cmd);
			eeprom_write(ROBOT_EEPROM_ADDRESS_STOP, eeprom_stop_cmd);
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

static uint8_t get_start() {
	return flag_start_cmd;
}

static uint8_t get_stop() {
	return flag_stop_cmd;
}

static void ready_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_READY);

	motors_set(0, 0);
}

static void ready_execute() {
	
	const uint8_t (*color1)[3] = &array[((12*ready_color_counter)/ROBOT_READY_COLOR_COUNTER_MAX)%12];
	const uint8_t (*color2)[3] = &array[(((12*ready_color_counter)/ROBOT_READY_COLOR_COUNTER_MAX) + 1)%12];

	const uint16_t fraq = (12*ready_color_counter)%ROBOT_READY_COLOR_COUNTER_MAX;

	const uint8_t r = (((uint16_t)(*color2)[0])*fraq + ((uint16_t)(*color1)[0])*(ROBOT_READY_COLOR_COUNTER_MAX - fraq))/ROBOT_READY_COLOR_COUNTER_MAX;
	const uint8_t g = (((uint16_t)(*color2)[1])*fraq + ((uint16_t)(*color1)[1])*(ROBOT_READY_COLOR_COUNTER_MAX - fraq))/ROBOT_READY_COLOR_COUNTER_MAX;
	const uint8_t b = (((uint16_t)(*color2)[2])*fraq + ((uint16_t)(*color1)[2])*(ROBOT_READY_COLOR_COUNTER_MAX - fraq))/ROBOT_READY_COLOR_COUNTER_MAX;
	
	ws2812b_set(r/8, g/8, b/8);

	ready_color_counter++;
	ready_color_counter %=ROBOT_READY_COLOR_COUNTER_MAX;
}

static void program_enter() {
	motors_set(0, 0);

	ws2812b_set(255, 255, 0);
	_delay_ms(250);
	ws2812b_set(0, 0, 0);
	_delay_ms(250);

	ws2812b_set(255, 255, 0);
	_delay_ms(250);
	ws2812b_set(0, 0, 0);
	_delay_ms(250);
}

static void run_enter() {
	ws2812b_set(0, 255, 0);

	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_RUN);

	//fight_init();
}

static void run_execute() {
	ws2812b_set(0, 255, 0);

	//fight_update();
}

static void stop1_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_STOP1);

	motors_set(0, 0);

	ws2812b_set(255, 32, 0);

	_delay_ms(1000);
}

static void stop2_enter() {
	eeprom_write(ROBOT_EEPROM_ADDRESS_STATE, ROBOT_STATE_READY);

	motors_set(0, 0);

	ws2812b_set(255, 0, 0);
}

void robot_init() {

	fsm_init(&fsm);

	fsm_define_state(&fsm, ROBOT_STATE_READY,		ready_enter,	ready_execute,	NULL);
	fsm_define_state(&fsm, ROBOT_STATE_PROGRAM,		program_enter,	NULL,			NULL);
	fsm_define_state(&fsm, ROBOT_STATE_RUN,			run_enter,		run_execute,	NULL);
	fsm_define_state(&fsm, ROBOT_STATE_STOP1,		stop1_enter,	NULL,			NULL);
	fsm_define_state(&fsm, ROBOT_STATE_STOP2,		stop2_enter,	NULL,			NULL);

	fsm_define_transition(&fsm, ROBOT_STATE_READY,		ROBOT_STATE_PROGRAM,	NULL, get_program);
	fsm_define_transition(&fsm, ROBOT_STATE_PROGRAM,	ROBOT_STATE_READY,		NULL, NULL);
	fsm_define_transition(&fsm, ROBOT_STATE_READY,		ROBOT_STATE_RUN,		NULL, get_start);
	fsm_define_transition(&fsm, ROBOT_STATE_RUN,		ROBOT_STATE_PROGRAM,	NULL, get_program);
	fsm_define_transition(&fsm, ROBOT_STATE_RUN,		ROBOT_STATE_STOP1,		NULL, get_stop);
	fsm_define_transition(&fsm, ROBOT_STATE_STOP1,		ROBOT_STATE_STOP2,		NULL, NULL);

	eeprom_start_cmd = eeprom_read(ROBOT_EEPROM_ADDRESS_START);
	eeprom_stop_cmd = eeprom_read(ROBOT_EEPROM_ADDRESS_STOP);

	fsm_start(&fsm, eeprom_read(ROBOT_EEPROM_ADDRESS_STATE));

	scheduler_add_task(loop, 1);
}
