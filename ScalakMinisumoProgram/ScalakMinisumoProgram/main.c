#define F_CPU 8000000UL

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "hardware/include/uart.h"
#include "hardware/include/ws2812b.h"

int main() {
    
	WS2812B_t led;
	
	uart_init(38400);
	ws2812b_init(&led, 1);
	
	uint8_t x = 0;	
	
    while(1) {

		printf("Hello world! %d\n", x);
		
		led.array[0] = (ColorRGB_t){0, 255, x};
			
		x++;
		
		ws2812b_show(&led);
		
		_delay_ms(10);
    }
}

