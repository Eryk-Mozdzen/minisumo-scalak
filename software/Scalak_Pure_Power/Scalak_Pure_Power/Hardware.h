#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

class Timer {
	public:
		Timer();
		unsigned long get();
		void increment(float f);
	private:
		unsigned long t;
		float fraction;
};

class Input {
	public:
		Input();
		Input(volatile uint8_t*, volatile uint8_t*, volatile uint8_t*, uint8_t);
		void initialize();
		bool get();
	private:
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		volatile uint8_t* PIN;
		uint8_t PINX;
};

class Output {
	public:
		Output();
		Output(volatile uint8_t*, volatile uint8_t*, uint8_t);
		void initialize();
		void set(bool);
		bool getState();
		void refresh(unsigned long);
		void blink(float);
	private:
		volatile uint8_t* DDR;
		volatile uint8_t* PORT;
		uint8_t PINX;
		bool state;
		unsigned long lastT;
		float frequancy;
};

class Analog {
	public:
		Analog();
		Analog(int);
		void initialize();
		int get();
	private:
		int index;
};

#endif