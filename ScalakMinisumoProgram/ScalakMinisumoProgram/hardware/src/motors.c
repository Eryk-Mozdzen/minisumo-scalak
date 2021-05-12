/*
 * motors.c
 *
 * Created: 03.05.2021 12:54:04
 *  Author: ermoz
 */ 

#include "../include/motors.h"

void motors_init(Motors_t *motors, uint8_t reverse) {

	motors->config = 0x00 | (reverse<<4);
	motors->velocity[0] = 0;
	motors->velocity[1] = 0;
	
	DDRD |= (1<<PIND6) | (1<<PIND7);
	DDRB |= (1<<PINB1) | (1<<PINB2);
	
	TCCR0A |= (1<<COM0A1) | (1<<WGM00) | (1<<WGM01);
	TCCR0B |= (1<<CS00);
	
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	
	motors_set_velocity(motors, MOTOR_L | MOTOR_R, 0);
}

void motors_set_velocity(Motors_t *motors, uint8_t target, float velocity) {	
	if(target & MOTOR_L) {
		motors->velocity[0] = fabs(velocity);
		if(velocity<0)	motors->config |= MOTOR_L;
		else			motors->config &= ~(MOTOR_L);
		
		OCR0A = motors->velocity[0];
		if((motors->config & MOTOR_L)^(motors->config & (MOTOR_L<<4)))
			PORTD |= (1<<PIND7);
		else
			PORTD &= ~(1<<PIND7);
	}
	
	if(target & MOTOR_R) {
		motors->velocity[1] = fabs(velocity);
		if(velocity<0)	motors->config |= MOTOR_R;
		else			motors->config &= ~(MOTOR_R);
		
		OCR1A = motors->velocity[1];
		if((motors->config & MOTOR_R)^(motors->config & (MOTOR_R<<4)))
			PORTB |= (1<<PINB2);
		else
			PORTB &= ~(1<<PINB2);
	}
}