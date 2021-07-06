#ifndef RC5_H_
#define RC5_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

class TransmitterRC5 {
	public:
		TransmitterRC5();
		void initialize();

		void send(uint8_t, uint8_t, uint8_t);
	private:
		void sendOne();
		void sendZero();
};

#endif