#ifndef F_CPU
	#define F_CPU 8000000
#endif

#define LED_STRIP_PORT PORTD
#define LED_STRIP_DDR  DDRD
#define LED_STRIP_PIN  3

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

class rgb_color {
	public:
	unsigned char red, green, blue;
	rgb_color() {}
	rgb_color(int r, int g, int b) {
		red = g;
		green = r;
		blue = b;
	}
	void set(int r, int g, int b) {
		red = g;
		green = r;
		blue = b;
	}
};

class Leds {
	public:
		Leds() {}
		Leds(int number_a) {
			number = number_a;
			//colors = new rgb_color[number];
		}
		void set(int r, int g, int b) {
			for(int i=0; i<number; i++)
				colors[i].set(r, g, b);
		}
		void set(int index, int r, int g, int b) {
			if(index>=0 && index<number)
				colors[index].set(r, g, b);
		}
		void set(int index,rgb_color color) {
			if(index>=0 && index<number)
				colors[index] = color;
		}
		void set(rgb_color(*func)(int, double), double variable) {
			for(int i=0; i<number; i++)
				set(i, func(i, variable));
		}
		void show() {
			led_strip_write(colors, number);
		}
		int getNumber() {
			return number;
		}
	private:
		int number;
		rgb_color colors[20];
		
		void __attribute__((noinline)) led_strip_write(rgb_color*, unsigned int);
};