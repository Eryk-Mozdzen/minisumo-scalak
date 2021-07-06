#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "WS2812B.h"

Leds light(5);

int getAnalog(int index) {
	ADMUX &= ~0x0F;
	ADMUX |= (index & 0x07);
	
	ADCSRA = ADCSRA | (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADCH;
}

rgb_color smoothErrorColor(int index, double error) {

	int number = light.getNumber();

	double dist = fabs(error*((number-1)/2.0) - double(index-((number-1)/2.0)));

	int r = 255 - dist*100;

	if(r>255) r = 255;
	if(r<0) r = 0;

	if(index==4) return rgb_color(0,r,255-r);
	return rgb_color(r, 0, 255 - r);
} 

int main() {

	ADMUX |= (1<<REFS0) | (1<<ADLAR);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	int input = 0;

	while(true) {

		input++;
		input %=255;
		double error = -(input - 127.5)/127.5;
		
		//double error = -(getAnalog(0) - 127.5)/127.5;

		light.set(smoothErrorColor, error);

		light.show();

		_delay_ms(10);
	}
}
