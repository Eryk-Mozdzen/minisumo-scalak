#include "RC5.h"

TransmitterRC5::TransmitterRC5() {}
	
void TransmitterRC5::initialize() {
	/*DDRD |= (1<<PIND5);
	OCR1A = 209;
	TCCR1B |= (1<<WGM12);
	TCNT1 = 0;*/
	
	DDRD |= (1<<PIND7);
	OCR2 = 210;
}

void TransmitterRC5::send(uint8_t toggle, uint8_t address, uint8_t command) {
	uint16_t message = 0;
	
	message |= (1<<13) | (1<<12);
	message |= (toggle<<11);
	message |= (address<<6);
	message |= (command);
	
	/*for(int i=13; i>=0; i--) {
		if(message & (1<<i)) sendOne();
		else sendZero();
	}*/
	
	for(int i=13; i>=0; i--) {
		if(message & (1<<i)) {
			TCCR2= (1<<FOC2)|(1<<COM21)|(1<<CS20);
			_delay_ms(0.89);
			TCCR2= (1<<FOC2)|(1<<WGM21)|(1<<COM20)|(1<<CS20);
			_delay_ms(0.89);
		} else {
			TCCR2= (1<<FOC2)|(1<<WGM21)|(1<<COM20)|(1<<CS20);
			_delay_ms(0.89);
			TCCR2= (1<<FOC2)|(1<<COM21)|(1<<CS20);
			_delay_ms(0.89);
		}
	}
	TCCR2= (1<<FOC2)|(1<<COM21)|(1<<CS20);
	TCCR2= 0;
}

void TransmitterRC5::sendOne() {
	_delay_us(897);
	TCCR1A |= (1<<COM1A0);
	TCCR1B |= (1<<CS10);
	_delay_us(897);
	TCCR1B &= ~(1<<CS10);
	TCCR1A &= ~(1<<COM1A0);
	PORTD &= ~(1<<PIND5);
	TCNT1 = 0;
}

void TransmitterRC5::sendZero() {
	TCNT1 = 0;
	TCCR1A |= (1<<COM1A0);
	TCCR1B |= (1<<CS10);
	_delay_us(897);
	TCCR1B &= ~(1<<CS10);
	TCCR1A &= ~(1<<COM1A0);
	PORTD &= ~(1<<PIND5);
	TCNT1 = 0;
	_delay_us(897);
}