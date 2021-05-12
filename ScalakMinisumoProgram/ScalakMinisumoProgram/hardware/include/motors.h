/*
 * motors.h
 *
 * Created: 03.05.2021 12:39:59
 *  Author: ermoz
 */ 


#ifndef MOTORS_H_
#define MOTORS_H_

#include <math.h>
#include <stdint.h>
#include <avr/io.h>

#define MOTOR_L			0x01
#define MOTOR_R			0x02

/*	config register:
		7 bit - not used
		6 bit - not used
		5 bit - reverse right motor
		4 bit - reverse left motor
		3 bit - not used
		2 bit - not used
		1 bit - direction right motor
		0 bit - direction left motor		*/

typedef struct {
	uint8_t config;
	uint8_t velocity[2];
} Motors_t;

void motors_init(Motors_t*, uint8_t);
void motors_set_velocity(Motors_t*, uint8_t, float);

#endif /* MOTORS_H_ */