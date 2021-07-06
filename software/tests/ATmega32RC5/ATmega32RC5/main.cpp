#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "RC5.h"

Stream serial;
DecoderRC5 ir;

ISR(INT0_vect) {
	ir.INT_ISR();
}

ISR(TIMER1_OVF_vect) {
	ir.TIMER_ISR();
}

int main() {

	serial.begin(9600);
	ir.initialize();

	while(true) {

		if(ir.newMessage()) {
			uint8_t toggle = ir.getToggle();
			uint8_t address = ir.getAddress();
			uint8_t command = ir.getCommand();
			
			//serial.print("Toggle: \0");
			serial.print(toggle);
			serial.print("\tAddress: \0");
			serial.print(address);
			serial.print("\tCommand: \0");
			serial.println(command);
		}
		
	}
}