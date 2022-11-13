#include "rc5.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "state_machine.h"

#define RC5_TIME_SHORT		889		// us
#define RC5_TIME_LONG		1778	// us
#define RC5_TIME_TOLERANCE	444		// us
#define RC5_TIME_PRESCALER	16		// us/LSB

typedef enum {
	RC5_STATE_START1,
	RC5_STATE_MID1,
	RC5_STATE_START0,
	RC5_STATE_MID0,
	RC5_STATE_RESET
} rc5_state_t;

static volatile uint8_t state;
static volatile uint16_t counter;
static volatile uint8_t bits_ready;
static volatile rc5_message_t message;
static fsm_t fsm;

static void emit1() {
	message |=(1<<(13 - bits_ready));
	bits_ready++;
}

static void emit0() {
	message &=~(1<<(13 - bits_ready));
	bits_ready++;
}

static void reset() {
	message = 0;
	bits_ready = 0;
}

static uint8_t get_short_space() {

	if(state) {
		int16_t time = counter*RC5_TIME_PRESCALER;

		if(time>=(RC5_TIME_SHORT - RC5_TIME_TOLERANCE) && time<=(RC5_TIME_SHORT + RC5_TIME_TOLERANCE))
			return 1;
	}

	return 0;
}

static uint8_t get_short_pulse() {

	if(!state) {
		int16_t time = counter*RC5_TIME_PRESCALER;

		if(time>=(RC5_TIME_SHORT - RC5_TIME_TOLERANCE) && time<=(RC5_TIME_SHORT + RC5_TIME_TOLERANCE))
			return 1;
	}

	return 0;
}

static uint8_t get_long_space() {

	if(state) {
		int16_t time = counter*RC5_TIME_PRESCALER;

		if(time>=(RC5_TIME_LONG - RC5_TIME_TOLERANCE) && time<=(RC5_TIME_LONG + RC5_TIME_TOLERANCE))
			return 1;
	}

	return 0;
}

static uint8_t get_long_pulse() {

	if(!state) {
		int16_t time = counter*RC5_TIME_PRESCALER;

		if(time>=(RC5_TIME_LONG - RC5_TIME_TOLERANCE) && time<=(RC5_TIME_LONG + RC5_TIME_TOLERANCE))
			return 1;
	}

	return 0;
}

void rc5_init() {

	fsm_add_state(&fsm, RC5_STATE_START1,	NULL,	NULL, NULL);
	fsm_add_state(&fsm, RC5_STATE_MID1,		emit1,	NULL, NULL);
	fsm_add_state(&fsm, RC5_STATE_START0,	NULL,	NULL, NULL);
	fsm_add_state(&fsm, RC5_STATE_MID0,		emit0,	NULL, NULL);
	fsm_add_state(&fsm, RC5_STATE_RESET,	reset,	NULL, NULL);

	fsm_add_transition(&fsm, RC5_STATE_START1,	RC5_STATE_MID1,		get_short_space);
	fsm_add_transition(&fsm, RC5_STATE_MID1,	RC5_STATE_START1,	get_short_pulse);
	fsm_add_transition(&fsm, RC5_STATE_MID1,	RC5_STATE_MID0,		get_long_pulse);
	fsm_add_transition(&fsm, RC5_STATE_MID0,	RC5_STATE_MID1,		get_long_space);
	fsm_add_transition(&fsm, RC5_STATE_MID0,	RC5_STATE_START0,	get_short_space);
	fsm_add_transition(&fsm, RC5_STATE_START0,	RC5_STATE_MID0,		get_short_pulse);
	fsm_add_transition(&fsm, RC5_STATE_RESET,	RC5_STATE_MID1,		NULL);

	fsm_start(&fsm, RC5_STATE_RESET);

	DDRD &=~(1<<DDD2);
	PORTD |=(1<<PORTD2);

	EIMSK |=(1<<INT0);
	EICRA |=(1<<ISC00);
	EICRA &=~(1<<ISC01);
	
	TCCR2B |=(1<<CS22) | (1<<CS20);
	TIMSK2 |=(1<<TOIE2);
	
	sei();
}

uint8_t rc5_get_message(rc5_message_t *msg) {
	if(bits_ready!=14)
		return 0;

	*msg = message;

	fsm_start(&fsm, RC5_STATE_RESET);

	return 1;
}

ISR(TIMER2_OVF_vect) {
	TCCR2B &=~((1<<CS22) | (1<<CS20));

	if(bits_ready==14)
		return;

	fsm_start(&fsm, RC5_STATE_RESET);
}

ISR(INT0_vect) {
	if(bits_ready==14)
		return;

	state = !(PIND & (1<<PIND2));
	counter = TCNT2;
	
	TCNT2 = 0;
	TCCR2B |=(1<<CS22) | (1<<CS20);

	fsm_update(&fsm);

	if(bits_ready==2 && RC5_MESSAGE_START(message)!=3)
		fsm_start(&fsm, RC5_STATE_RESET);
}
