#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "src/pid.h"
#include "src/rc5.h"
#include "src/uart.h"
#include "src/ws2812b.h"
#include "scalak_lib.h"
#include "program_state_machine.h"
#include "tactics_state_machine.h"

float color_counter = 0;

rgb_color color_function(float value, float MIN, float MAX) {
	
	rgb_color array[] = {
		{255,   0, 255},
		{255,   0, 127},
		{255,   0,   0},
		{255, 127,   0},
		{255, 255,   0},
		{127, 255,   0},
		{  0, 255,   0},
		{  0, 255, 127},
		{  0, 255, 255},
		{  0, 127, 255},
		{  0,   0, 255},
		{127,   0, 255}
	};
	uint8_t array_size = sizeof(array)/sizeof(rgb_color);
	
	rgb_color result = (rgb_color){0, 0, 0};
		
	float fraq = (value-MIN)/(MAX+1-MIN);
	float index = fraq*array_size;
	rgb_color low = array[(uint8_t)floor(index)];
	rgb_color high = array[((uint8_t)ceil(index))%array_size];
	float p = index - floor(index);
	
	result.r = (1-p)*low.r + p*high.r;
	result.g = (1-p)*low.g + p*high.g;
	result.b = (1-p)*low.b + p*high.b;
	
	return result;
}

int main() {
	
	uart_init(38400);
	decoder_rc5_initialize();
	pid_initialize(0, 127.5 + 50, 0, 0);
	
	drv8838_initialize(FORWARD, BACKWARD);
	qtr1a_initialize(AUTO, 500);
	ir38khz_initialize();
	switch_initialize();
	button_initialize();
	
	set_commands(eeprom_read(ADDRESS_DOHYO_IDENTIFIER));
	set_program_state(eeprom_read(ADDRESS_PROGRAM_STATE));
	set_tactics_state(eeprom_read(ADDRESS_TACTICS_STATE));
	
	drv8838_initialize(OFF, OFF);
	
    while(1) {
		update_program_state_rc5();
		//update_program_state_button();
		//update_program_state_module();

		switch(current_program_state) {
			case PROGRAM_STATE_PROGRRAMING:	{
					drv8838_set_speeds(0, 0);
					rgb_color_set_rgb(255, 165, 0);		_delay_ms(50);
					rgb_color_set_rgb(0, 0, 0);			_delay_ms(50);
					rgb_color_set_rgb(255, 165, 0);		_delay_ms(50);
					rgb_color_set_rgb(0, 0, 0);			_delay_ms(50);
					
					//rotate(M_PI/4, 0, TACTICS_EXPLORE_POWER);
					//rotate(M_PI/2, 0, TACTICS_EXPLORE_POWER);
					//rotate(3*M_PI/4, 0, TACTICS_EXPLORE_POWER);
					//rotate(M_PI, 0, TACTICS_EXPLORE_POWER);
					
				} break;
			case PROGRAM_STATE_POWER_ON: {
					drv8838_set_speeds(0, 0);
					rgb_color_set(rgb_color_multiply(color_function(color_counter, 0, 1000), 0.3));
					color_counter +=5;
					if(color_counter>1000) color_counter = 0;
				} break;
			case PROGRAM_STATE_STARTED:	{
					rgb_color_set_rgb(0, 100, 0);
				
					//main program
				
					main_program();
				
				} break;
			case PROGRAM_STATE_STOPPED_SAFE: {
					drv8838_set_speeds(0, 0);
					rgb_color_set_rgb(100, 0, 0);
				} break;
			case PROGRAM_STATE_STOPPED: {} break;
			default: {} break;
		}
		
		printf("\n");
		
		printf("Program: %u Tactics: %u\n", current_program_state, current_tactics_state);
		
		printf("Motors:\t\t%d\t%d\n", drv8838_get_speed(LEFT), drv8838_get_speed(RIGHT));
		printf("Proximity:\t%u%u%u%u%u\n", ir38khz_get_state(0), ir38khz_get_state(1), ir38khz_get_state(2), ir38khz_get_state(3), ir38khz_get_state(4));
		printf("Line:\t\t%u%u\t%u\t%u\n", qtr1a_get_state(LEFT), qtr1a_get_state(RIGHT), qtr1a_get_state_raw(LEFT), qtr1a_get_state_raw(RIGHT));
		//printf("Switch: %u\tButton: %u\n", switch_get_state(), button_get_state());
		
		_delay_ms(50);
    }
}