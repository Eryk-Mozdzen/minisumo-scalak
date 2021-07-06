#ifndef RC5_H_
#define RC5_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SHORT_MIN	2*444
#define SHORT_MAX	2*1333
#define LONG_MIN	2*1334
#define LONG_MAX	2*2222

#define STATE_START1	0
#define STATE_MID1		1
#define STATE_MID0		2
#define STATE_START0	3
#define STATE_ERROR		4
#define STATE_BEGIN		5
#define STATE_END		6

class DecoderRC5 {
	public:
		DecoderRC5();
		void initialize();
		void reset();
		void INT_ISR();
		void TIMER_ISR();
		
		bool newMessage();
		uint8_t getToggle();
		uint8_t getAddress();
		uint8_t getCommand();
	private:
		volatile bool isNew;
		volatile uint16_t message;
		int currentState;
		int counter;
		uint16_t delayFull;
};

#endif