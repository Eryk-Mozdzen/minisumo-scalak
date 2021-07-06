#include "Hardware.h"

Timer::Timer() {
	t = 0;
	fraction = 0;
}

unsigned long Timer::get() {
	if(fraction>1) {
		t +=fraction;
		fraction = 0;
	}
	return t;
}

void Timer::increment(float f) {
	fraction +=f;
}

Input::Input() {}

Input::Input(volatile uint8_t* DDR_a, volatile uint8_t* PORT_a, volatile uint8_t* PIN_a, uint8_t PINX_a) {
	DDR = DDR_a;
	PORT = PORT_a;
	PIN = PIN_a;
	PINX = PINX_a;
}

void Input::initialize() {
	*DDR &= ~(1<<PINX);
	*PORT |= (1<<PINX);
}

bool Input::get() {
	return (*PIN & (1<<PINX));
}

Output::Output() {}

Output::Output(volatile uint8_t* DDR_a, volatile uint8_t* PORT_a, uint8_t PINX_a) {
	DDR = DDR_a;
	PORT = PORT_a;
	PINX = PINX_a;
	state = false;
	lastT = 0;
	frequancy = 0;
}

void Output::initialize() {
	*DDR |= 1<<PINX;
	*PORT &= ~(1<<PINX);
	
	Output::set(false);
}

void Output::set(bool state_a) {
	state = state_a;
	frequancy = 0;
	
	if(state) *PORT |= 1<<PINX;
	else *PORT &= ~(1<<PINX);
}

bool Output::getState() {
	return state;
}

void Output::refresh(unsigned long millis) {
	if(frequancy>0) {
		float peroid = 1000.0/frequancy;
		if((lastT+(peroid)/2.0)<=millis) {
			*PORT ^= 1<<PINX;
			lastT = millis;
		}
	}
}

void Output::blink(float frequancy_a) {
	frequancy = frequancy_a;
}

Analog::Analog() {}
	
Analog::Analog(int index_a) {
	index = index_a;
}
	
void Analog::initialize() {
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);
}

int Analog::get() {
	ADMUX &= ~0x0F;
	ADMUX |= (index & 0x07);
	
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADC;
}