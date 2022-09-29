#include "periph.h"

static uint16_t line_raw[2];

static void reader_task() {
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

	scheduler_add_task(34, reader_task, 100/33);
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
	dest[0] = !(PINC & (1<<PINC3));
	dest[1] = !(PINC & (1<<PINC4));
	dest[2] = !(PINC & (1<<PINC5));
	dest[3] = !(PINB & (1<<PINB7));
	dest[4] = !(PINB & (1<<PINB6));
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
