#include "RC5.h"

DecoderRC5::DecoderRC5() {}

void DecoderRC5::initialize() {
	DDRD &= ~(1<<PIND2);
	PORTD |= (1<<PIND2);
	EIMSK |= (1<<INT0);

	//change
	EICRA |= (1<<ISC00);
	EICRA &= ~(1<<ISC01);
	
	TCCR2B |= (1 << CS20);
	TIMSK2 |= (1<<TOIE2);
	
	sei();
	
	DecoderRC5::reset();
}

void DecoderRC5::reset() {
	isNew = false;
	counter = 14;
	message = 0;
	delayFull = 0;
	currentState = STATE_BEGIN;
}

void DecoderRC5::TIMER_ISR() {
	delayFull++;
}

void DecoderRC5::INT_ISR() {
	uint16_t delay = ((TCNT2 + delayFull*256)>>2);
	
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
		TCNT2 = 0;
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
	}
	
	TCNT2 = 0;
	delayFull = 0;
}

bool DecoderRC5::newMessage() {
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