//Scalak_lib.h by Eryk Mozdzen 15.12.2019

#ifndef SCALAK_LIB_H_
#define SCALAK_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "UART.h"
#include "RC5.h"

#define LEFT		0
#define RIGHT		1
#define FORWARD		0
#define BACKWARD	1
#define NORMAL		0
#define INVERT		1
#define AUTO		2
#define SINGLE		0
#define MULTI		1
#define ON			0
#define OFF			1
#define MANUAL		20
#define MODULE		21

#define STATE_PROGRAMING	0x01
#define STATE_POWER_ON		0x02
#define STATE_STARTED		0x03
#define STATE_STOPPED_SAFE	0x04
#define STATE_STOPPED		0x05

#define SAVED_STATE			0x0A
#define DYHLO_IDENTIFIER	0x0B

#define ADDRESS_PROGRAMING	0x0B
#define ADDRESS_START_STOP	0x07

void EEPROMwrite(unsigned short, unsigned char);
unsigned char EEPROMread(unsigned char);

class drv8838 {
	public:
		drv8838();
		void initialize(uint8_t, uint8_t);
		void setSpeeds(uint8_t, uint8_t);
		void setSpeeds(uint8_t);
		void print(Stream*);
		void stop();
		int getSpeed(uint8_t);
		static uint8_t fix(uint8_t);
	private:
		uint8_t power[2];
		bool rotate[2];

		void setSpeed(uint8_t, uint8_t);
		void flip(uint8_t);
};

class IR38kHz {
	public:
		IR38kHz();
		void initialize();
		bool getState(uint8_t);
		float getError(uint8_t);
		void print(Stream*);
	private:
		bool getStateRAW(uint8_t);
		float getErrorSingle();
		float getErrorMulti();
};

class QTR1A {
	public:
		QTR1A();
		void initialize(uint8_t, uint8_t);
		bool getState(uint8_t);
		uint8_t getHorizon();
		void print(Stream*);
	private:
		bool inv;
		uint8_t trigger;

		uint16_t getStateRAW(uint8_t);
		void setMode(uint8_t);
};

class Buzzer {
	public:
		Buzzer();
		void initialize();
		void play();
		void stop();
};

class StartModule {
	public:
		StartModule();
		void initialize(uint8_t);
		void wait(Stream*, DecoderRC5*, void(*)(), void(*)());
		void print();
		uint8_t getMode();
		uint8_t getState();
	private:
		uint8_t mode;
		bool debug;
		bool clicked;
		
		unsigned char currentState;
		uint8_t stopCommand, startCommand;
		bool prog;

		bool read();
		void setMode(uint8_t);
		void waitModule(Stream*, DecoderRC5*, void(*)(), void(*)());
		void waitManual(Stream*, void(*)(), void(*)());
		void printDebug(Stream*);
		
		void setState(unsigned char);
		void setCommands(uint8_t);
		void update(DecoderRC5*);
};

class Switch {
	public:
		Switch();
		void initialize();
		bool getState();
		void print(Stream*);
};
/*
class Led {
	public:
		Led();
		Led(int);
		void initialize();
		void blink(float);
		void setState(int);
	private:
		int pin;
		unsigned long lastT;
};
*/

#endif