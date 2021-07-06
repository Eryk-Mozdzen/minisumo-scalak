#include "RC5.h"

DecoderRC5::DecoderRC5() {}

void DecoderRC5::initialize() {
	DDRB &= ~(1<<PIND2);
	PORTB |= 1<<PIND2;
	GICR |=1<<INT0;
	
	//any change
	MCUCR |=1<<ISC00;
	MCUCR &= ~(1<<ISC01);
	
	//pre-scalar 8 => ticT = 500us
	TCCR1A = 0x00;
	TCCR1B = (1<<CS11);
	TCCR1B &= ~((1<<CS10) | (1<<CS12));
	TCNT1 = 0;
	
	TIMSK = 1<<TOIE1;
	
	sei();
	
	DecoderRC5::reset();
}

void DecoderRC5::reset() {
	isNew = false;
	counter = 14;
	message = 0;
	delayFull = 0;
	currentState = STATE_BEGIN;

	//GICR |=1<<INT0;
}

void DecoderRC5::TIMER_ISR() {
	delayFull++;
}

void DecoderRC5::INT_ISR() {
	uint16_t delay = TCNT1 + delayFull*65535;
	
	/*
	0 - short space
	2 - short pulse
	4 - long space
	6 - long pulse
	*/
	
	uint8_t event = (PIND & (1<<PIND2)) ? 2 : 0;
	
	if(delay>LONG_MIN && delay<LONG_MAX) event +=4;
	else if(delay<SHORT_MIN || delay>SHORT_MAX) DecoderRC5::reset();
	
	if(currentState==STATE_BEGIN) {
		counter--;
		message |= (1<<counter);
		currentState = STATE_MID1;
		TCNT1 = 0;
		return;
	}
	
	const uint8_t trans[4] = {0x01, 0x91, 0x9b, 0xfb};
	int newState = (trans[currentState]>>event) & 0x03;

    if(newState==currentState || currentState>STATE_START0) {
        DecoderRC5::reset();
        return;
    }
    
    currentState = newState;
	
	if(currentState==STATE_MID0) counter--;
	else if(currentState==STATE_MID1) {
		counter--;
		message |= (1<<counter);
	}
	
	if(counter==0 && (currentState==STATE_START1 || currentState==STATE_MID0)) {
		currentState = STATE_END;
		isNew = true;
		//GICR &= ~(1<<INT0);
	}
	
	TCNT1 = 0;
	delayFull = 0;
}

bool DecoderRC5::newMessage() {
	//if(isNew) DecoderRC5::reset();
	bool result = isNew;
	isNew = false;
	return result;
}

uint8_t DecoderRC5::getToggle() {
	return (uint8_t)((message & 0x0800)>>11);
}

uint8_t DecoderRC5::getAddress() {
	return (uint8_t)((message & 0x07C0)>>6);
}

uint8_t DecoderRC5::getCommand() {
	return (uint8_t)(message & 0x003F);
}