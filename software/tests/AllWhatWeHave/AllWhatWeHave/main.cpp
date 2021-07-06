#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "WS2812B.h"
#include "UART.h"
#include "irmp.h"

static void timer1_init (void) {
	#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
		#if F_CPU >= 16000000L
			OCR1C   =  (F_CPU / F_INTERRUPTS / 8) - 1;
			TCCR1   = (1 << CTC1) | (1 << CS12);
		#else
			OCR1C   =  (F_CPU / F_INTERRUPTS / 4) - 1;
			TCCR1   = (1 << CTC1) | (1 << CS11) | (1 << CS10);
		#endif
	#else
		OCR1A   =  (F_CPU / F_INTERRUPTS) - 1;
		TCCR1B  = (1 << WGM12) | (1 << CS10);
	#endif
	#ifdef TIMSK1
		TIMSK1  = 1 << OCIE1A;
	#else
		TIMSK   = 1 << OCIE1A;
	#endif
}

#ifdef TIM1_COMPA_vect
	#define COMPA_VECT  TIM1_COMPA_vect
#else
	#define COMPA_VECT  TIMER1_COMPA_vect
#endif

ISR(COMPA_VECT) {
	(void) irmp_ISR();
}

void initAnalog() {
	ADMUX |= (1<<REFS0) | (1<<ADLAR);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

int getAnalog(int index) {
	ADMUX &= ~0x0F;
	ADMUX |= (index & 0x07);
	
	ADCSRA = ADCSRA | (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADCH;
}

Stream serial;
//Leds light(4);

int main() {
	
	serial.begin(9600);
	initAnalog();
	
	IRMP_DATA irmp_data;
	irmp_init();
	timer1_init();
	sei();

    while(true) {

		if(irmp_get_data (&irmp_data)) {
			serial.print("Address: \0");
			serial.print(irmp_data.address);
			serial.print("\tCommand: \0");
			serial.println(irmp_data.command);
		}
		
    }
}

