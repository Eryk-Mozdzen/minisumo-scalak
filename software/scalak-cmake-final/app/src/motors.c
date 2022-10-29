#include "motors.h"

static int16_t power[2];
static int16_t limit;

void motors_init() {

	DDRD |=(1<<PIND6) | (1<<PIND7);
	DDRB |=(1<<PINB1) | (1<<PINB2);
	
	TCCR1A |=(1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);
	TCCR1B |=(1<<CS10) | (1<<WGM12);

	limit = 255;
}

void motors_limit(uint8_t lim) {
	limit = lim;
}

void motors_set(int16_t left, int16_t right) {
	if(left>limit)
		left = limit;
	if(left<-limit)
		left = -limit;
	if(right>limit)
		right = limit;
	if(right<-limit)
		right = -limit;

	power[0] = left;
	power[1] = right;

	// set left motor
	if(power[0]>0) {
		OCR1A = power[0];
		PORTD &=~(1<<PIND7);
	} else {
		OCR1A = -power[0];
		PORTD |=(1<<PIND7);
	}

	// set right motor
	if(power[1]>0) {
		OCR1B = power[1];
		PORTD |=(1<<PIND6);
	} else {
		OCR1B = -power[1];
		PORTD &=~(1<<PIND6);
	}
}

void motors_get(int16_t *dest) {
	dest[0] = power[0];
	dest[1] = power[1];
}
