#ifndef TACTICS_STATE_MACHINE_H_
#define TACTICS_STATE_MACHINE_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pid.h"
#include "rc5.h"
#include "uart.h"
#include "ws2812b.h"
#include "scalak_lib.h"
#include "program_state_machine.h"

#define ADDRESS_TACTICS_STATE		0x0C

typedef enum {
	TACTICS_STATE_UNNAMED = 0x00,
	TACTICS_STATE_BEGIN = 0xBE,
	TACTICS_STATE_FIGHT = 0xF1,
	TACTICS_STATE_FOLLOW = 0xF0,
	TACTICS_STATE_EXPLORE = 0xEE
} tactics_state;

#define TACTICS_FIGHT_POWER			255
#define TACTICS_FOLLOW_POWER		128
#define TACTICS_FOLLOW_TIME			20				// 100 for 10ms loop delay
#define TACTICS_EXPLORE_POWER		25
#define TACTICS_EXPLORE_TIME_STEP	0.01
#define TACTICS_TURN_MAX_DIST		0.015

tactics_state current_tactics_state;
uint16_t enemy_follow_counter = 0;
uint8_t last_enemy_direction;

void set_tactics_state(tactics_state state) {
	enemy_follow_counter = 0;
	current_tactics_state = state;
	eeprom_write(ADDRESS_TACTICS_STATE, current_tactics_state);
}

void update_tactics_state(float dist_error, uint8_t edge_output[]) {
	//set_tactics_state(TACTICS_STATE_EXPLORE);
	//return;
	
	switch(current_tactics_state) {
		case TACTICS_STATE_BEGIN: {
			
			set_tactics_state(TACTICS_STATE_FOLLOW);
			
		} break;
		case TACTICS_STATE_FIGHT: {
			
			if(isnan(dist_error))
				set_tactics_state(TACTICS_STATE_FOLLOW);
				
		} break;
		case TACTICS_STATE_FOLLOW: {
			
			if(!isnan(dist_error))
				set_tactics_state(TACTICS_STATE_FIGHT);
			else if(edge_output[0] | edge_output[1])
				set_tactics_state(TACTICS_STATE_EXPLORE);
			else if(enemy_follow_counter<TACTICS_FOLLOW_TIME)
				enemy_follow_counter++;
			else
				set_tactics_state(TACTICS_STATE_EXPLORE);
				
		} break;
		case TACTICS_STATE_EXPLORE: {
			
			if(!isnan(dist_error))
				set_tactics_state(TACTICS_STATE_FIGHT);
				
		} break;
		default: {
			
			set_tactics_state(TACTICS_STATE_BEGIN);
			
		} break;
	}
}

void main_program() {
	float dist_error = ir38khz_get_error();
	uint8_t edge_output[] = {qtr1a_get_state(LEFT), qtr1a_get_state(RIGHT)};
	
	update_tactics_state(dist_error, edge_output);
	
	switch(current_tactics_state) {
		case TACTICS_STATE_BEGIN: {
			last_enemy_direction = !switch_get_state();
		} break;
		case TACTICS_STATE_FIGHT: {
			last_enemy_direction = (dist_error>0);

			double turn = pid_calculate(0, dist_error, 0);

			int16_t powerL = drv8838_fix(TACTICS_FIGHT_POWER + turn);
			int16_t powerR = drv8838_fix(TACTICS_FIGHT_POWER - turn);

			drv8838_set_speeds(powerL, powerR);
		} break;
		case TACTICS_STATE_FOLLOW: {
			int16_t powerL = 0;
			int16_t powerR = 0;

			/*if(edge_output[0]==0 && edge_output[1]==0) {}
			if(edge_output[0]==0 && edge_output[1]==1) { last_enemy_direction = 0; powerL -=150; }
			if(edge_output[0]==1 && edge_output[1]==0) { last_enemy_direction = 1; powerR -=150; }
			if(edge_output[0]==1 && edge_output[1]==1) {}*/

			if(last_enemy_direction) powerL = TACTICS_FOLLOW_POWER;
			else powerR = TACTICS_FOLLOW_POWER;

			pid_reset(0);
			drv8838_set_speeds(powerL, powerR);
		} break;
		case TACTICS_STATE_EXPLORE: {
			//if(last_enemy_direction) drv8838_set_speeds(-50, 50);
			//else drv8838_set_speeds(50, -50);
			
			drv8838_set_speeds(TACTICS_EXPLORE_POWER, TACTICS_EXPLORE_POWER);
			
			// TURNING PROCEDURE blocking state
			
			if(qtr1a_get_state(LEFT) || qtr1a_get_state(RIGHT)) {
				printf("Line detected! Start distance measurement\n");
				
				uint8_t dir = qtr1a_get_state(LEFT);
				float distance = 0;
				
				while((!qtr1a_get_state(LEFT) || !qtr1a_get_state(RIGHT)) && distance<TACTICS_TURN_MAX_DIST) {
					_delay_ms(TACTICS_EXPLORE_TIME_STEP*1000);
					
					distance +=(TACTICS_EXPLORE_TIME_STEP*TACTICS_EXPLORE_POWER*POWER_SCALE*WHEEL_RADIUS*2*M_PI);
					
					printf("Line detected! Measurement distance\n");
				}
				
				drv8838_set_speeds(0, 0);
				_delay_ms(100);
				
				float phi = M_PI - atan2(distance, EDGE_SPACING);
				
				printf("Measured distance:\t%f\n", distance);
				printf("Estimated angle:\t%f\n", phi*180.f/M_PI);
				
				drv8838_set_speeds(-TACTICS_EXPLORE_POWER, -TACTICS_EXPLORE_POWER);
				_delay_ms(300);
				drv8838_set_speeds(0, 0);
				_delay_ms(100);
				
				rotate(phi, dir, TACTICS_EXPLORE_POWER);
			}
			
			pid_reset(0);
			
		} break;
		default: {} break;
	}
}

#endif