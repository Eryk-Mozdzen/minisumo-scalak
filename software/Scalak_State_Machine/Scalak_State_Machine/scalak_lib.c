#include <math.h>
#include <avr/io.h>
#include "scalak_lib.h"

void drv8838_initialize(uint8_t flip1, uint8_t flip2) {
	drv8838_rotate[0] = flip1;
	drv8838_rotate[1] = flip2;
	drv8838_power[0] = 0;
	drv8838_power[1] = 0;
	
	DDRD |= (1<<PIND6) | (1<<PIND7);
	DDRB |= (1<<PINB1) | (1<<PINB2);
	
	TCCR0A |= (1<<COM0A1) | (1<<WGM00) | (1<<WGM01);
	TCCR0B |= (1<<CS00);
	
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	
	drv8838_set_speeds(0, 0);
}

void drv8838_set_speeds(int16_t speed1, int16_t speed2) {
	drv8838_set_speed(LEFT, speed1);
	drv8838_set_speed(RIGHT, speed2);
}

int16_t drv8838_get_speed(uint8_t index) {
	if(index==LEFT) return drv8838_power[0];
	if(index==RIGHT) return drv8838_power[1];
	return -1;
}

int16_t drv8838_fix(int16_t input) {
	if(input>255) input = 255;
	if(input<-255) input = -255;
	return input;
}

void drv8838_set_speed(uint8_t index, int16_t speed) {
	if(index==LEFT) drv8838_power[0] = drv8838_fix(speed);
	if(index==RIGHT) drv8838_power[1] = drv8838_fix(speed);
	
	switch(index) {
		case RIGHT: {
				/*OCR0A = fabs(drv8838_power[1]);
				if(drv8838_power[1]!=0) {
					if((drv8838_rotate[1]==FORWARD)==(drv8838_power[1]>0)) PORTD |= (1<<PIND7);
					else PORTD &= ~(1<<PIND7);
				}*/
				
				if(drv8838_rotate[1]==OFF) {
					OCR0A = 0x00;
					break;
				}
				
				OCR0A = fabs(drv8838_power[1]);
				if((drv8838_rotate[1]==FORWARD)==(drv8838_power[1]>0))
					PORTD |= (1<<PIND7);
				else
					PORTD &= ~(1<<PIND7);
				
				} break; 
		case LEFT: {
				/*OCR1A = fabs(drv8838_power[0]);
				if(drv8838_power[0]!=0) {
					if((drv8838_rotate[0]==FORWARD)==(drv8838_power[0]>0)) PORTB |= (1<<PINB2);
					else PORTB &= ~(1<<PINB2);
				}*/
				
				if(drv8838_rotate[0]==OFF) {
					OCR1A = 0x00;
					break;
				}
				
				OCR1A = fabs(drv8838_power[0]);
				if((drv8838_rotate[0]==FORWARD)==(drv8838_power[0]>0))
					PORTB |= (1<<PINB2);
				else
					PORTB &= ~(1<<PINB2);
				
			}	break;
		default: break;
	}
}

void ir38khz_initialize() {
	DDRC &= ~(1<<PINC3) & ~(1<<PINC4) & ~(1<<PINC5);
	DDRB &= ~(1<<PINB6) & ~(1<<PINB7);
	
	PORTC |= (1<<PINC3) | (1<<PINC4) | (1<<PINC5);
	PORTB |= (1<<PINB6) | (1<<PINB7);
}

uint8_t ir38khz_get_state_raw(uint8_t index) {
	switch(index) {
		case 0: return !(PINC & (1<<PINC3));
		case 1: return !(PINC & (1<<PINC4));
		case 2: return !(PINC & (1<<PINC5));
		case 3: return !(PINB & (1<<PINB7));
		case 4: return !(PINB & (1<<PINB6));
	}
	return 0;
}

uint8_t ir38khz_get_state(uint8_t index) {
	int good = 0;
	for(int i=0; i<10; i++)
	good +=ir38khz_get_state_raw(index);
	return (good>=7);
	//return IR38kHz::getStateRAW(index);
}

float ir38khz_get_error() {
	int active = 0;
	int sum = 0;

	for(int i=0; i<5; i++) {
		if(ir38khz_get_state(i)) {
			active++;
			sum +=(i-2);
		}
	}

	return (float)(sum)/(float)(active);
}

void qtr1a_initialize(uint8_t mode, uint16_t trigger_a) {
	//DDRC &= ~(1<<PINC1) & ~(1<<PINC2);
	
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);
	
	qtr1a_inv = 1;
	qtr1a_trigger = trigger_a;
	
	qtr1a_set_mode(mode);
}

uint16_t qtr1a_get_state_raw(uint8_t index) {
	ADMUX &= ~0x0F;
	switch(index) {
		case LEFT:	ADMUX |= (2 & 0x07);	break;
		case RIGHT:	ADMUX |= (1 & 0x07);	break;
	}
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

uint8_t qtr1a_get_state(uint8_t index) {
	int good = 0;
	for(int i=0; i<5; i++)
		good +=(qtr1a_inv==(qtr1a_get_state_raw(index)<qtr1a_trigger));
	return (good>=3);
	//return (inv==(QTR1A::getStateRAW(index)<trigger));
}

void qtr1a_set_mode(uint8_t mode) {
	switch(mode) {
		case BLACK: {	qtr1a_inv = 1; } break;
		case WHITE: {	qtr1a_inv = 0; } break;
		case AUTO: {
			uint8_t left = qtr1a_get_state(LEFT);
			uint8_t right = qtr1a_get_state(RIGHT);
			
			if(left==right) {
				qtr1a_inv = !left;
			} else {
				qtr1a_set_mode(BLACK);
			}
		} break;
		default: {
			qtr1a_set_mode(BLACK);
		} break;
	}
}

uint8_t qtr1a_get_horizon() {
	return ((qtr1a_get_state(LEFT)<<1) | qtr1a_get_state(RIGHT));
}

void switch_initialize() {
	DDRB &= ~(1<<PINB4);
	PORTB |= (1<<PINB4);
}

uint8_t switch_get_state() {
	return !(PINB & (1<<PINB4));
}

void button_initialize() {
	DDRC &= ~(1<<PINC0);
	PORTC |= (1<<PINC0);
}

uint8_t button_get_state() {
	return !(PINC & (1<<PINC0));
}

void eeprom_write(uint8_t address, uint8_t data) {
	while(EECR & (1<<EEPE)) {}
	
	EECR = (0<<EEPM1) | (0<<EEPM0);
	
	if(address<512) EEAR = address;
	else EEAR = 511;
	
	EEDR = data;
	
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

uint8_t eeprom_read(uint8_t address) {
	while(EECR & (1<<EEPE)) {}
	
	EEAR = address;
	
	EECR |= (1<<EERE);
	
	return EEDR;
}

void rotate(float phi, uint8_t dir, int16_t speed) {
	double S = phi*(WHEEL_SPACING/2)*2;
	double l = 2*M_PI*WHEEL_RADIUS;
	double rounds = S/l;
	double rotateTime = rounds/(speed*POWER_SCALE);
	
	if(dir) drv8838_set_speeds(speed, -speed);
	else drv8838_set_speeds(-speed, speed);
	
	for(int i=0; i<rotateTime*100; i++)
		_delay_ms(10);
	
	drv8838_set_speeds(0, 0);
	_delay_ms(100);
}