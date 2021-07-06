//Scalak_lib.cpp by Eryk Mozdzen 15.12.2019

#include "Scalak_lib.h"

drv8838::drv8838() {
	rotate[LEFT] = true;
	power[LEFT] = 0;
	rotate[RIGHT] = true;
	power[RIGHT] = 0;
}

void drv8838::initialize(uint8_t flip1, uint8_t flip2) {
	DDRD |= (1<<PIND6) | (1<<PIND7);
	DDRB |= (1<<PINB1) | (1<<PINB2);
	
	TCCR0A |= (1<<COM0A1) | (1<<WGM00) | (1<<WGM01);
	TCCR0B |= (1<<CS00);
	
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);
	TCCR1B |= (1<<CS10) | (1<<WGM12);

	if(flip1==BACKWARD) drv8838::flip(LEFT);
	if(flip2==BACKWARD) drv8838::flip(RIGHT);
	
	drv8838::stop();
}

void drv8838::setSpeeds(uint8_t speed1, uint8_t speed2) {
	drv8838::setSpeed(LEFT, speed1);
	drv8838::setSpeed(RIGHT, speed2);
}

void drv8838::setSpeeds(uint8_t speed) {
	drv8838::setSpeed(LEFT, speed);
	drv8838::setSpeed(RIGHT, speed);
}

void drv8838::flip(uint8_t index) {
	rotate[index] = !rotate[index];
}

void drv8838::print(Stream* serial) {
	for(int i=0; i<2; i++) {
		serial->print(power[i]);
		serial->print("\t");
	}
	serial->println("\t");
}

void drv8838::stop() {
	drv8838::setSpeed(LEFT, 0);
	drv8838::setSpeed(RIGHT, 0);
}

int drv8838::getSpeed(uint8_t index) {
	return power[index];
}

uint8_t drv8838::fix(uint8_t input) {
	if(input>255) input = 255;
	if(input<-255) input = -255;
	return input;
}

void drv8838::setSpeed(uint8_t index, uint8_t speed) {
	power[index] = drv8838::fix(speed);
	
	switch(index) {
		case RIGHT: {
			OCR0A = fabs(power[index]);
			if(power[index]!=0) {
				if(rotate[index]==(power[index]>0)) PORTD |= (1<<PIND7);
				else PORTD &= ~(1<<PIND7);
			}	break; }
		case LEFT: {
			OCR1A = fabs(power[index]);
			if(power[index]!=0) {
				if(rotate[index]==(power[index]>0)) PORTB |= (1<<PINB2);
				else PORTB &= ~(1<<PINB2);
			}	break; }
		default: break;
	}
}

IR38kHz::IR38kHz() {}

void IR38kHz::initialize() {
	DDRC &= ~(1<<PINC3) & ~(1<<PINC4) & ~(1<<PINC5);
	DDRB &= ~(1<<PINB6) & ~(1<<PINB7);
	
	PORTC |= (1<<PINC3) | (1<<PINC4) | (1<<PINC5);
	PORTB |= (1<<PINB6) | (1<<PINB7);
}

bool IR38kHz::getStateRAW(uint8_t index) {
	switch(index) {
		case 0: return !(PINC & (1<<PINC3));
		case 1: return !(PINC & (1<<PINC4));
		case 2: return !(PINC & (1<<PINC5));
		case 3: return !(PINB & (1<<PINB7));
		case 4: return !(PINB & (1<<PINB6));
	}
}

bool IR38kHz::getState(uint8_t index) {
	int good = 0;
	for(int i=0; i<10; i++)
		good +=IR38kHz::getStateRAW(index);
	return (good>=7);
	//return IR38kHz::getStateRAW(index);
}

float IR38kHz::getError(uint8_t mode) {
	switch(mode) {
		case SINGLE:	return IR38kHz::getErrorSingle();
		case MULTI:		return IR38kHz::getErrorMulti();
		default: break;
	}
}

float IR38kHz::getErrorSingle() {
	int active = 0;
	int sum = 0;

	for(int i=0; i<5; i++) {
		if(IR38kHz::getState(i)) {
			active++;
			sum +=(i-2);
		}
	}

	return (float)(sum)/(float)(active);
}

float IR38kHz::getErrorMulti() {
	int active = 0;
	int sum = 0;
	float smallest = 1000;

	for(int i=0; i<5; i++) {
		if(IR38kHz::getState(i)) {
			active++;
			sum +=(i-2);
		}
		if((!IR38kHz::getState(i) && active!=0) || i==4) {
			float error = (float)(sum)/(float)(active);
			if(fabs(error)<fabs(smallest))
			smallest = error;
			active = 0;
			sum = 0;
		}
	}

	if(smallest!=1000) return smallest;
	else return 0.0/0.0;
}

void IR38kHz::print(Stream* serial) {
	for(int i=0; i<5; i++)
		serial->print(IR38kHz::getState(i));
	serial->println("\r");
}

QTR1A::QTR1A() {
	inv = true;
}

void QTR1A::initialize(uint8_t mode, uint8_t trigger_a) {
	//DDRC &= ~(1<<PINC1) & ~(1<<PINC2);
	
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);
	
	trigger = trigger_a;
	
	QTR1A::setMode(mode);
}

uint16_t QTR1A::getStateRAW(uint8_t index) {
	ADMUX &= ~0x0F;
	switch(index) {
		case LEFT:	ADMUX |= (2 & 0x07);	break;
		case RIGHT:	ADMUX |= (1 & 0x07);	break;
	}
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

bool QTR1A::getState(uint8_t index) {
	int good = 0;
	for(int i=0; i<5; i++)
		good +=(inv==(QTR1A::getStateRAW(index)<trigger));
	return (good>=3);
	//return (inv==(QTR1A::getStateRAW(index)<trigger));
}

void QTR1A::setMode(uint8_t mode) {
	switch(mode) {
		case NORMAL: {	inv = true;		break; }
		case INVERT: {	inv = false;	break; }
		case AUTO: {
			bool left = QTR1A::getState(LEFT);
			bool right = QTR1A::getState(RIGHT);
			if(left==right) inv = !left;
			break; }
		default: { break; }
	}
}

uint8_t QTR1A::getHorizon() {
	return 2*QTR1A::getState(LEFT) + QTR1A::getState(RIGHT);
}

void QTR1A::print(Stream* serial) {
	serial->print(QTR1A::getState(LEFT));
	serial->print(QTR1A::getState(RIGHT));
	serial->print("\t");
	serial->print(QTR1A::getStateRAW(LEFT));
	serial->print("\t");
	serial->print(QTR1A::getStateRAW(RIGHT));
	serial->println("\t");
}

StartModule::StartModule() {
	mode = OFF;
	debug = false;
	clicked = false;
	prog = false;
}

void StartModule::initialize(uint8_t mode) {
	DDRC &= ~(1<<PINC0);
	PORTC |= (1<<PINC0);
	
	setMode(mode);

	setState(EEPROMread(SAVED_STATE));
	setCommands(EEPROMread(DYHLO_IDENTIFIER));
	
	if(mode==MANUAL && (currentState==STATE_PROGRAMING || currentState==STATE_STOPPED_SAFE || currentState==STATE_STOPPED))
		setState(STATE_POWER_ON);
}

void StartModule::wait(Stream* serial, DecoderRC5* decoder, void(*func)(), void(*quitFunc)()) {
	switch(mode) {
		case OFF: break;
		case MODULE:	StartModule::waitModule(serial, decoder, func, quitFunc);	break;
		case MANUAL:	StartModule::waitManual(serial, func, quitFunc);			break;
		default: break;
	}
	
	printDebug(serial);
}

void StartModule::waitModule(Stream* serial, DecoderRC5* decoder, void(*func)(), void(*quitFunc)()) {
	StartModule::update(decoder);
	
	while(currentState!=STATE_STARTED) {
		func();
		StartModule::update(decoder);
		if(currentState==STATE_STARTED)
			quitFunc();	

		printDebug(serial);
		_delay_ms(10);
	}
}

void StartModule::update(DecoderRC5* decoder) {
	if(decoder->newMessage()) {
		uint8_t address = decoder->getAddress();
		uint8_t command = decoder->getCommand();

		switch(currentState) {
			case STATE_POWER_ON:
				if(address==ADDRESS_START_STOP && command==startCommand) setState(STATE_STARTED);
				if(address==ADDRESS_START_STOP && command==stopCommand) setState(STATE_STOPPED_SAFE);
				if(address==ADDRESS_PROGRAMING) {
					setState(STATE_PROGRAMING);
					setCommands(command);
					prog = true;
				}	break;
			case STATE_STARTED:
				if(address==ADDRESS_START_STOP && command==stopCommand) setState(STATE_STOPPED_SAFE);
				if(address==ADDRESS_PROGRAMING) {
					setState(STATE_PROGRAMING);
					setCommands(command);
					prog = true;
				}	break;
			default: break;
		}
	}
	switch(currentState) {
		case STATE_PROGRAMING:
			if(prog) prog = false;
			else setState(STATE_POWER_ON);	break;
		case STATE_STOPPED_SAFE:	setState(STATE_STOPPED);	break;
		default: break;
	}
}

void StartModule::waitManual(Stream* serial, void(*func)(), void(*quitFunc)()) {
	if(!StartModule::read()) clicked = false;
	if(StartModule::read() && !clicked) {
		clicked = true;
		if(currentState==STATE_POWER_ON) setState(STATE_STARTED);
		else setState(STATE_POWER_ON);
	}
	while(currentState==STATE_POWER_ON) {
		func();

		if(!StartModule::read()) clicked = false;
		if(StartModule::read() && !clicked) {
			clicked = true;
			if(currentState==STATE_POWER_ON) setState(STATE_STARTED);
			else setState(STATE_POWER_ON);
		}
		
		if(currentState==STATE_STARTED)
			quitFunc();
			
		printDebug(serial);
		_delay_ms(10);
	}
}

void StartModule::setState(unsigned char state) {
	currentState = state;
	switch(currentState) {
		case STATE_PROGRAMING:		EEPROMwrite(SAVED_STATE, STATE_POWER_ON);		break;
		case STATE_POWER_ON:		EEPROMwrite(SAVED_STATE, STATE_POWER_ON);		break;
		case STATE_STARTED:			EEPROMwrite(SAVED_STATE, STATE_STARTED);		break;
		case STATE_STOPPED_SAFE:	EEPROMwrite(SAVED_STATE, STATE_STOPPED_SAFE);	break;
		case STATE_STOPPED:			EEPROMwrite(SAVED_STATE, STATE_POWER_ON);		break;
		default:					setState(STATE_PROGRAMING);						break;
	}
}

void StartModule::setCommands(uint8_t command) {
	stopCommand = command & 0xFE;
	startCommand = (command & 0xFE) + 1;
	
	EEPROMwrite(DYHLO_IDENTIFIER, stopCommand);
}

void StartModule::printDebug(Stream* serial) {
	if(debug) {
		switch(mode) {
			case OFF: serial->println("Start Module not used\r"); break;
			case MODULE: 
				switch(currentState) {
					case STATE_PROGRAMING:		serial->println("Module PROGRAMING\r");		break;
					case STATE_POWER_ON:		serial->println("Module POWER ON\r");		break;
					case STATE_STARTED:			serial->println("Module STARTED\r");		break;
					case STATE_STOPPED_SAFE:	serial->println("Module STOPPED SAFE\r");	break;
					case STATE_STOPPED:			serial->println("Module STOPPED\r");		break;
				}	break;
			case MANUAL:
				switch(currentState) {
					case STATE_PROGRAMING:		serial->println("Manual PROGRAMING\r");		break;
					case STATE_POWER_ON:		serial->println("Manual POWER ON\r");		break;
					case STATE_STARTED:			serial->println("Manual STARTED\r");		break;
					case STATE_STOPPED_SAFE:	serial->println("Manual STOPPED SAFE\r");	break;
					case STATE_STOPPED:			serial->println("Manual STOPPED\r");		break;
				}	break;
			default: break;
		}
	}
}

void StartModule::setMode(uint8_t mode_a) {
	mode = mode_a;

	if(mode==AUTO) {
		if(StartModule::read()) mode = MODULE;
		else mode = MANUAL;
	}
}

void StartModule::print() {
	debug = true;
}

bool StartModule::read() {
	return !(PINC & (1<<PINC0));
}

uint8_t StartModule::getMode() {
	return mode;
}

uint8_t StartModule::getState() {
	return currentState;
}

Buzzer::Buzzer() {}

void Buzzer::initialize() {
	DDRB |= (1<<PINB0);
	PORTB &= ~(1<<PINB0);
	
	Buzzer::stop();
}

void Buzzer::play() {
	PORTB |= (1<<PINB0);
}

void Buzzer::stop() {
	PORTB &= ~(1<<PINB0);
}

Switch::Switch() {}

void Switch::initialize() {
	DDRB &= ~(1<<PINB4);
	PORTB |= (1<<PINB4);
}

bool Switch::getState() {
	return !(PINB & (1<<PINB4));
}

void Switch::print(Stream* serial) {
	serial->println(Switch::getState());
}

void EEPROMwrite(unsigned short address, unsigned char data) {
	while(EECR & (1<<EEPE)) {}
	
	EECR = (0<<EEPM1) | (0<<EEPM0);
	
	if(address<512) EEAR = address;
	else EEAR = 511;
	
	EEDR = data;
	
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

unsigned char EEPROMread(unsigned char address) {
	while(EECR & (1<<EEPE)) {}
	
	EEAR = address;
	
	EECR |= (1<<EERE);
	
	return EEDR;
}