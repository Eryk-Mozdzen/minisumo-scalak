#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "RC5.h"
#include "UART.h"

class Input {
	public:
		
		Input() {}
		
		Input(volatile unsigned char *DDR_a, volatile unsigned char *PORT_a, unsigned char PIN_a) {
			DDR = DDR_a;
			PORT = PORT_a;
			PIN = PIN_a;
		}
		
		void initialize() {
			*DDR &= ~(1<<PIN);
			*PORT |= (1<<PIN);
		}
		
		bool get() {
			return (*PORT & (1<<PIN));
		}
	
	private:
		volatile unsigned char *DDR;
		volatile unsigned char *PORT;
		unsigned char PIN;
};

Stream serial;
TransmitterRC5 transmiter;
Input stopButton(&DDRA, &PORTA, PINA0);
Input startButton(&DDRA, &PORTA, PINA1);
Input programButton(&DDRA, &PORTA, PINA2);
Input addressSwitch[5] = {
	Input(&DDRC, &PORTC, PINC0), 
	Input(&DDRC, &PORTC, PINC1),
	Input(&DDRC, &PORTC, PINC2),
	Input(&DDRC, &PORTC, PINC3),
	Input(&DDRC, &PORTC, PINC4)
};

int main(void) {
	
	serial.begin(9600);
	transmiter.initialize();
	stopButton.initialize();
	startButton.initialize();
	programButton.initialize();
	for(int i=0; i<5; i++)
		addressSwitch[i].initialize();
	
    while(true) {
		
		transmiter.send(0, 0x0B, 0);
		
		//serial.println(TransmitterRC5::makeData(0, 0x0B, 5));
		
		_delay_ms(1000);
    }
}

