#define F_CPU 8000000UL

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "hardware/include/uart.h"
#include "hardware/include/ws2812b.h"

int main() {
    
	WS2812B_t led;
	
	uart_init(38400);
	ws2812b_init(&led, 1);
	
    while(1) {

		ws2812b_rainbow_step(&led, 0.1, 10);
		
		//led.array[0] = (ColorRGB_t){255, 0, 0};
		//led.array[0] = (ColorRGB_t){0, 255, 0};
		//led.array[0] = (ColorRGB_t){0, 0, 255};
			
		//printf("%d\t%d\t%d\n", led.array[0].r, led.array[0].g, led.array[0].b);
		
		ws2812b_show(&led);
		
		_delay_ms(10);
    }
}

