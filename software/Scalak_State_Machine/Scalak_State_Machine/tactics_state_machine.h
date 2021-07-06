#ifndef TACTICS_STATE_MACHINE_H_
#define TACTICS_STATE_MACHINE_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "src/pid.h"
#include "src/rc5.h"
#include "src/uart.h"
#include "src/ws2812b.h"
#include "scalak_lib.h"
#include "program_state_machine.h"

#define ADDRESS_TACTICS_STATE		0x0C

enum tactics_state {
	TACTICS_STATE_UNNAMED = 0x00,
	TACTICS_STATE_BEGIN = 0x01,
	TACTICS_STATE_FIGHT = 0x02,
	TACTICS_STATE_FOLLOW = 0x03,
	TACTICS_STATE_CUSTOM = 0x04
};

#define TACTICS_TARGET_POWER		255.0
#define TACTICS_FOLLOW_TIME			100
#define TACTICS_EXPLORE_POWER		50
#define TACTICS_TIME_STEP			0.001

enum tactics_state current_tactics_state;
uint16_t enemy_follow_counter = 0;
bool last_enemy_direction;

void set_tactics_state(enum tactics_state state) {
	enemy_follow_counter = 0;
	current_tactics_state = state;
	eeprom_write(ADDRESS_TACTICS_STATE, current_tactics_state);
}

void update_tactics_state(double dist_error, bool edge_output[]) {
	switch(current_tactics_state) {
		case TACTICS_STATE_BEGIN: {
			set_tactics_state(TACTICS_STATE_FOLLOW);
		} break;
		case TACTICS_STATE_FIGHT: {
			if(isnan(dist_error)) set_tactics_state(TACTICS_STATE_FOLLOW);
		} break;
		case TACTICS_STATE_FOLLOW: {
			if(!isnan(dist_error)) set_tactics_state(TACTICS_STATE_FIGHT);
			else if(enemy_follow_counter<TACTICS_FOLLOW_TIME) enemy_follow_counter++;
			else set_tactics_state(TACTICS_STATE_CUSTOM);
		} break;
		case TACTICS_STATE_CUSTOM: {
			if(!isnan(dist_error)) set_tactics_state(TACTICS_STATE_FIGHT);
		} break;
		default: set_tactics_state(TACTICS_STATE_BEGIN); break;
	}
}

void main_program() {
	double dist_error = ir38khz_get_error();
	bool edge_output[] = {qtr1a_get_state(LEFT), qtr1a_get_state(RIGHT)};
	
	update_tactics_state(dist_error, edge_output);
	
	switch(current_tactics_state) {
		case TACTICS_STATE_BEGIN: {
			last_enemy_direction = switch_get_state();
		} break;
		case TACTICS_STATE_FIGHT: {
			last_enemy_direction = (dist_error>0);

			double turn = pid_calculate(0, dist_error, 0);

			int16_t powerL = drv8838_fix(TACTICS_TARGET_POWER + turn);
			int16_t powerR = drv8838_fix(TACTICS_TARGET_POWER - turn);

			drv8838_set_speeds(powerL, powerR);
		} break;
		case TACTICS_STATE_FOLLOW: {
			int16_t powerL = 0;
			int16_t powerR = 0;

			if(edge_output[0]==0 && edge_output[1]==0) {}
			if(edge_output[0]==0 && edge_output[1]==1) { last_enemy_direction = false; powerL -=150; }
			if(edge_output[0]==1 && edge_output[1]==0) { last_enemy_direction = true;  powerR -=150; }
			if(edge_output[0]==1 && edge_output[1]==1) {}

			if(last_enemy_direction) powerL = 255;
			else powerR = 255;

			pid_reset(0);
			drv8838_set_speeds(powerL, powerR);
		} break;
		case TACTICS_STATE_CUSTOM: {
			//if(last_enemy_direction) drv8838_set_speeds(-50, 50);
			//else drv8838_set_speeds(50, -50);
			
			drv8838_set_speeds(TACTICS_EXPLORE_POWER, TACTICS_EXPLORE_POWER);
			
			if(edge_output[0] || edge_output[1]) {
				bool dir = edge_output[0];
				double distance = 0;
				while(edge_output[0] && edge_output[1]) {
					_delay_ms(TACTICS_TIME_STEP*1000);
					distance +=(TACTICS_TIME_STEP*TACTICS_EXPLORE_POWER*POWER_SCALE*WHEEL_RADIUS);
				}
				drv8838_set_speeds(0, 0);
				_delay_ms(100);
				double phi = M_PI - atan2(distance, EDGE_SPACING);
				
				drv8838_set_speeds(-TACTICS_EXPLORE_POWER, -TACTICS_EXPLORE_POWER);
				_delay_ms(300);
				drv8838_set_speeds(0, 0);
				_delay_ms(100);
				
				rotate(phi, dir, TACTICS_EXPLORE_POWER);
			}
			
		} break;
		default: break;
	}
}

#endif