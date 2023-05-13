#include "periph.h"
#include <avr/io.h>
#include "scheduler.h"

static uint16_t line_raw[2];
static uint16_t prox_hist[5];

void line_task() {
	// read left line
	ADMUX &=~0x0F;
	ADMUX |=(2 & 0x07);
	ADCSRA |=(1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	line_raw[0] = ADC;

	// read right line
	ADMUX &=~0x0F;
	ADMUX |=(1 & 0x07);
	ADCSRA |=(1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	line_raw[1] = ADC;
}

void prox_task() {
	prox_hist[0] = (prox_hist[0]<<1) | !(PINC & (1<<PINC3));
	prox_hist[1] = (prox_hist[1]<<1) | !(PINC & (1<<PINC4));
	prox_hist[2] = (prox_hist[2]<<1) | !(PINC & (1<<PINC5));
	prox_hist[3] = (prox_hist[3]<<1) | !(PINB & (1<<PINB7));
	prox_hist[4] = (prox_hist[4]<<1) | !(PINB & (1<<PINB6));
}

void periph_init() {
	// line
	ADMUX |=(1<<REFS0);
	ADCSRA |=(1<<ADEN);

	// proximity
	DDRC &=~((1<<PINC3) | (1<<PINC4) | (1<<PINC5));
	DDRB &=~((1<<PINB6) | (1<<PINB7));
	PORTC |=(1<<PINC3) | (1<<PINC4) | (1<<PINC5);
	PORTB |=(1<<PINB6) | (1<<PINB7);

	// switch
	DDRB &=~(1<<PINB4);
	PORTB |=(1<<PINB4);

	// button
	DDRC &=~(1<<PINC0);
	PORTC |=(1<<PINC0);

	// eeprom
	EECR &=~((1<<EEPM1) | (1<<EEPM0));
}

void line_get_raw(uint16_t *dest) {
	dest[0] = line_raw[0];
	dest[1] = line_raw[1];
}

void line_get(uint8_t *dest) {
	dest[0] = (line_raw[0]<LINE_THRESHOLD);
	dest[1] = (line_raw[1]<LINE_THRESHOLD);
}

void prox_get(uint8_t *dest) {
	for(uint8_t i=0; i<5; i++) {

		uint8_t num = 0;
		for(uint8_t j=0; j<16; j++) {
			if(prox_hist[i] & (1<<j))
				num++;
		}

		dest[i] = num>PROX_THRESHOLD;
	}
}

uint8_t switch_get() {
	return !(PINB & (1<<PINB4));
}

uint8_t button_get() {
	return !(PINC & (1<<PINC0));
}

uint8_t eeprom_read(uint16_t address) {
	while(EECR & (1<<EEPE));

	EEAR = address;

	EECR |=(1<<EERE);

	return EEDR;
}

void eeprom_write(uint16_t address, uint8_t src) {
	while(EECR & (1<<EEPE));
	
	EEAR = address;
	EEDR = src;
	
	EECR |=(1<<EEMPE);
	EECR |=(1<<EEPE);
}
