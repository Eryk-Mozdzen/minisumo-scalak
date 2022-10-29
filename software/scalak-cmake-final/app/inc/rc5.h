#ifndef RC5_H_
#define RC5_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "state_machine.h"

#define RC5_TIME_SHORT		889		// us
#define RC5_TIME_LONG		1778	// us
#define RC5_TIME_TOLERANCE	444		// us
#define RC5_TIME_PRESCALER	16		// us/LSB

typedef union {
	struct {
		uint16_t command : 6;
		uint16_t address : 5;
		uint16_t toggle : 1;
		uint16_t start : 2;
		uint16_t unused : 2;
	};
	uint16_t frame;
} rc5_message_t;

void rc5_init();
uint8_t rc5_get_message(rc5_message_t *);

ISR(INT0_vect);
ISR(TIMER2_OVF_vect);

#endif