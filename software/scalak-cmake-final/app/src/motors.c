#include "motors.h"

static int16_t power[2];

static void update_task() {

	// set left motor
	if(power[0]>0) {
		OCR1A = power[0];
		PORTB |=(1<<PINB2);
	} else {
		OCR1A = -power[0];
		PORTB &=~(1<<PINB2);
	}

	// set right motor
	if(power[1]<0) {
		OCR0A = -power[1];
		PORTD |=(1<<PIND7);
	} else {
		OCR0A = power[1];
		PORTD &=~(1<<PIND7);
	}
}

void motors_init() {

	DDRD |=(1<<PIND6) | (1<<PIND7);
	DDRB |=(1<<PINB1) | (1<<PINB2);
	
	TCCR0A |=(1<<COM0A1) | (1<<WGM00) | (1<<WGM01);
	
	TCCR1A |=(1<<COM1A1) | (1<<WGM10);
	TCCR1B |=(1<<CS10) | (1<<WGM12);

	scheduler_add_task(update_task, 1);
}

void motors_set(int16_t left, int16_t right) {
	if(left>255)
		left = 255;
	if(left<-255)
		left = -255;
	if(right>255)
		right = 255;
	if(right<-255)
		right = -255;

	power[0] = left;
	power[1] = right;
}

void motors_get(int16_t *dest) {
	dest[0] = power[0];
	dest[1] = power[1];
}
