/*
 * dist.c
 *
 * Created: 13.05.2021 00:03:45
 *  Author: ermoz
 */ 

#include "../include/dist.h"

void dist_init(Dist_t *dist, uint8_t not_active) {
	dist->active = ~not_active;
	
	DDRC &= ~(1<<PINC3) & ~(1<<PINC4) & ~(1<<PINC5);
	DDRB &= ~(1<<PINB6) & ~(1<<PINB7);
	
	PORTC |= (1<<PINC3) | (1<<PINC4) | (1<<PINC5);
	PORTB |= (1<<PINB6) | (1<<PINB7);
}

void dist_test(Dist_t *dist) {
	uint8_t read = dist_get_state(dist, DIST_ALL);
	printf("%d%d%d%d%d\n", 
		(read & DIST_L ) ? 1 : 0,
		(read & DIST_LF) ? 1 : 0,
		(read & DIST_F ) ? 1 : 0,
		(read & DIST_RF) ? 1 : 0,
		(read & DIST_R ) ? 1 : 0
	);
}

uint8_t dist_get_state(Dist_t *dist, uint8_t target) {
	uint8_t read = 0x00;
	
	if(target & DIST_L  & dist->active) read |= (PINC & (1<<PINC3));
	if(target & DIST_LF & dist->active) read |= (PINC & (1<<PINC4));
	if(target & DIST_F  & dist->active) read |= (PINC & (1<<PINC5));
	if(target & DIST_RF & dist->active) read |= (PINB & (1<<PINB7));
	if(target & DIST_R  & dist->active) read |= (PINB & (1<<PINB6));
	
	return read;
}

