#include "rc5.h"

void decoder_rc5_initialize() {
	DDRD &= ~(1<<PIND2);
	PORTD |= (1<<PIND2);
	EIMSK |= (1<<INT0);

	//change
	EICRA |= (1<<ISC00);
	EICRA &= ~(1<<ISC01);
	
	TCCR2B |= (1 << CS20);
	TIMSK2 |= (1<<TOIE2);
	
	sei();
	
	decoder_rc5_reset();
}

void decoder_rc5_reset() {
	decoder_rc5_isNew = false;
	decoder_rc5_counter = 14;
	decoder_rc5_message = 0;
	decoder_rc5_delayFull = 0;
	decoder_rc5_currentState = STATE_BEGIN;
}

void decoder_rc5_TIMER_ISR() {
	decoder_rc5_delayFull++;
}

void decoder_rc5_INT_ISR() {
	uint16_t delay = ((TCNT2 + decoder_rc5_delayFull*256)>>2);
	
	/*
	0 - short space
	2 - short pulse
	4 - long space
	6 - long pulse
	*/
	
	uint8_t event = (PIND & (1<<PIND2)) ? 2 : 0;
	
	if(delay>LONG_MIN && delay<LONG_MAX) event +=4;
	else if(delay<SHORT_MIN || delay>SHORT_MAX) decoder_rc5_reset();
	
	if(decoder_rc5_currentState==STATE_BEGIN) {
		decoder_rc5_counter--;
		decoder_rc5_message |= (1<<decoder_rc5_counter);
		decoder_rc5_currentState = STATE_MID1;
		TCNT2 = 0;
		return;
	}
	
	const uint8_t trans[4] = {0x01, 0x91, 0x9b, 0xfb};
	int newState = (trans[decoder_rc5_currentState]>>event) & 0x03;

    if(newState==decoder_rc5_currentState || decoder_rc5_currentState>STATE_START0) {
        decoder_rc5_reset();
        return;
    }
    
    decoder_rc5_currentState = newState;
	
	if(decoder_rc5_currentState==STATE_MID0) decoder_rc5_counter--;
	else if(decoder_rc5_currentState==STATE_MID1) {
		decoder_rc5_counter--;
		decoder_rc5_message |= (1<<decoder_rc5_counter);
	}
	
	if(decoder_rc5_counter==0 && (decoder_rc5_currentState==STATE_START1 || decoder_rc5_currentState==STATE_MID0)) {
		decoder_rc5_currentState = STATE_END;
		decoder_rc5_isNew = true;
	}
	
	TCNT2 = 0;
	decoder_rc5_delayFull = 0;
}

bool decoder_rc5_new_message() {
	bool result = decoder_rc5_isNew;
	decoder_rc5_isNew = false;
	return result;
}

uint8_t decoder_rc5_get_toggle() {
	return (uint8_t)((decoder_rc5_message & 0x0800)>>11);
}

uint8_t decoder_rc5_get_address() {
	return (uint8_t)((decoder_rc5_message & 0x07C0)>>6);
}

uint8_t decoder_rc5_get_command() {
	return (uint8_t)(decoder_rc5_message & 0x003F);
}

ISR(INT0_vect) {
	decoder_rc5_INT_ISR();
}

ISR(TIMER2_OVF_vect) {
	decoder_rc5_TIMER_ISR();
}