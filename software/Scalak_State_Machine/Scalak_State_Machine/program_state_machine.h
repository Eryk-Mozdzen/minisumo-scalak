#ifndef PROGRAM_STATE_MACHINE_H_
#define PROGRAM_STATE_MACHINE_H_

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
#include "tactics_state_machine.h"

#define RC5_COMMAND_PROGRAMING		0x0B
#define RC5_COMMAND_START_STOP		0x07

#define ADDRESS_PROGRAM_STATE		0x0A
#define ADDRESS_DYHLO_IDENTIFIER	0x0B

enum program_state {
	PROGRAM_STATE_PROGRRAMING = 0x00,
	PROGRAM_STATE_POWER_ON = 0x01,
	PROGRAM_STATE_STARTED = 0x02,
	PROGRAM_STATE_STOPPED_SAFE = 0x03,
	PROGRAM_STATE_STOPPED = 0x04
};

uint8_t stop_command, start_command;
enum program_state current_program_state;
bool button_last_state;

void set_program_state(enum program_state state) {
	current_program_state = state;
	switch(current_program_state) {
		case PROGRAM_STATE_PROGRRAMING:		eeprom_write(ADDRESS_PROGRAM_STATE, PROGRAM_STATE_POWER_ON);		break;
		case PROGRAM_STATE_POWER_ON:		eeprom_write(ADDRESS_PROGRAM_STATE, PROGRAM_STATE_POWER_ON);		break;
		case PROGRAM_STATE_STARTED:			eeprom_write(ADDRESS_PROGRAM_STATE, PROGRAM_STATE_STARTED);			break;
		case PROGRAM_STATE_STOPPED_SAFE:	eeprom_write(ADDRESS_PROGRAM_STATE, PROGRAM_STATE_STOPPED_SAFE);	break;
		case PROGRAM_STATE_STOPPED:			eeprom_write(ADDRESS_PROGRAM_STATE, PROGRAM_STATE_POWER_ON);		break;
		default:							set_program_state(PROGRAM_STATE_PROGRRAMING);						break;
	}
	set_tactics_state(TACTICS_STATE_UNNAMED);
}

void set_commands(uint8_t command) {
	stop_command = command & 0xFE;
	start_command = (command & 0xFE) + 1;
	eeprom_write(ADDRESS_DYHLO_IDENTIFIER, stop_command);
}

void update_program_state_rc5() {
	switch(current_program_state) {
		case PROGRAM_STATE_PROGRRAMING:		set_program_state(PROGRAM_STATE_POWER_ON);	break;
		case PROGRAM_STATE_STOPPED_SAFE:	set_program_state(PROGRAM_STATE_STOPPED);	break;
		default: break;
	}
	
	if(decoder_rc5_new_message()) {
		uint8_t address = decoder_rc5_get_address();
		uint8_t command = decoder_rc5_get_command();

		switch(current_program_state) {
			case PROGRAM_STATE_POWER_ON:
			if(address==RC5_COMMAND_START_STOP && command==start_command) set_program_state(PROGRAM_STATE_STARTED);
			if(address==RC5_COMMAND_START_STOP && command==stop_command) set_program_state(PROGRAM_STATE_STOPPED_SAFE);
			if(address==RC5_COMMAND_PROGRAMING) {
				set_program_state(PROGRAM_STATE_PROGRRAMING);
				set_commands(command);
			}	break;
			case PROGRAM_STATE_STARTED:
			if(address==RC5_COMMAND_START_STOP && command==stop_command) set_program_state(PROGRAM_STATE_STOPPED_SAFE);
			if(address==RC5_COMMAND_PROGRAMING) {
				set_program_state(PROGRAM_STATE_PROGRRAMING);
				set_commands(command);
			}	break;
			default: break;
		}
	}
	
}

void var_delay_ms(int16_t delay) {
	while(delay--)
		_delay_ms(1);
}

void update_program_state_button() {
	switch(current_program_state) {
		case PROGRAM_STATE_PROGRRAMING:		set_program_state(PROGRAM_STATE_POWER_ON);	break;
		case PROGRAM_STATE_STOPPED_SAFE:	set_program_state(PROGRAM_STATE_STOPPED);	break;
		default: break;
	}
	
	if(button_get_state() && !button_last_state) {
		switch(current_program_state) {
			case PROGRAM_STATE_POWER_ON: {
				
					int16_t time = 5000*(5.0/10.0);
					int16_t cycle = 250;
					
					while(time>=0) {
						if(cycle>50)
							cycle -=12;
						time -=cycle;
						
						for(int16_t i=0; i<cycle/2; i++) {
							double fraq = (double)i/(double)((cycle/2)-1);
							rgb_color_set_rgb(fraq*100, fraq*40, 0);
							_delay_ms(1);
						}
						
						for(int16_t i=0; i<cycle/2; i++) {
							double fraq = (double)i/(double)((cycle/2)-1);
							rgb_color_set_rgb((1-fraq)*100, (1-fraq)*40, 0);
							_delay_ms(1);
						}

					}
					
					set_program_state(PROGRAM_STATE_STARTED);
				}	break;
			case PROGRAM_STATE_STARTED:		set_program_state(PROGRAM_STATE_STOPPED_SAFE);	break;
			case PROGRAM_STATE_STOPPED:		set_program_state(PROGRAM_STATE_POWER_ON);	break;
			default: break;
		}
	}
	
	button_last_state = button_get_state();
}

void update_program_state_module() {
	switch(current_program_state) {
		case PROGRAM_STATE_PROGRRAMING:		set_program_state(PROGRAM_STATE_POWER_ON);	break;
		case PROGRAM_STATE_STOPPED_SAFE:	set_program_state(PROGRAM_STATE_POWER_ON);	break;
		default: break;
	}
	
	if(!button_get_state() && button_last_state) {
		set_program_state(PROGRAM_STATE_STARTED);
	}
	if(button_get_state() && !button_last_state) {
		set_program_state(PROGRAM_STATE_POWER_ON);
	}
	
	button_last_state = button_get_state();
}

#endif