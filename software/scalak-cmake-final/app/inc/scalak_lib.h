#ifndef SCALAK_LIB_H_
#define SCALAK_LIB_H_

#include <avr/io.h>
#include <util/delay.h>

#define POWER_SCALE		0.16			//0.16 for 12V & 50/255 motor power
#define WHEEL_RADIUS	0.0155
#define EDGE_SPACING	0.073
#define WHEEL_SPACING	0.075

#define LEFT		0x00
#define RIGHT		0x01

#define FORWARD		0x02
#define BACKWARD	0x03
#define	OFF			0x07

#define BLACK		0x04
#define WHITE		0x05
#define AUTO		0x06

uint8_t drv8838_rotate[2];
int16_t drv8838_power[2];
uint8_t qtr1a_inv;
uint16_t qtr1a_trigger;

void drv8838_initialize(uint8_t, uint8_t);
void drv8838_set_direction(uint8_t, uint8_t);
int16_t drv8838_get_speed(uint8_t);
int16_t drv8838_fix(int16_t);
void drv8838_set_speeds(int16_t, int16_t);
void drv8838_set_speed(uint8_t, int16_t);

void ir38khz_initialize();
uint8_t ir38khz_get_state_raw(uint8_t);
uint8_t ir38khz_get_state(uint8_t);
float ir38khz_get_error();

void qtr1a_initialize(uint8_t, uint16_t);
uint16_t qtr1a_get_state_raw(uint8_t);
uint8_t qtr1a_get_state(uint8_t);
void qtr1a_set_mode(uint8_t);
uint8_t qtr1a_get_horizon();

void switch_initialize();
uint8_t switch_get_state();

void button_initialize();
uint8_t button_get_state();

uint8_t eeprom_read(uint8_t);
void eeprom_write(uint8_t, uint8_t);

void rotate(float, uint8_t, int16_t);

#endif