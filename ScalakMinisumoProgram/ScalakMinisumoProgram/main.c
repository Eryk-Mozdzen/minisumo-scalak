#define F_CPU 8000000UL

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "hardware/include/uart.h"
#include "hardware/include/ws2812b.h"
#include "hardware/include/motors.h"
#include "hardware/include/dist.h"

int main() {
    
	WS2812B_t led;
	Motors_t motors;
	Dist_t dist;
	
	uart_init(38400);
	
	ws2812b_init(&led, 1);
	motors_init(&motors, MOTOR_L | MOTOR_R);
	dist_init(&dist, DIST_NONE);
	
    while(1) {

		ws2812b_rainbow_step(&led, 0.5, 10);
		
		//drv8838_set_velocity(&motor_l, 50);
		//drv8838_set_velocity(&motor_r, 50);
		
		//led.array[0] = (ColorRGB_t){255, 0, 0};
		//led.array[0] = (ColorRGB_t){0, 255, 0};
		//led.array[0] = (ColorRGB_t){0, 0, 255};
		
		//printf("%d\t%d\t%d\n", led.array[0].r, led.array[0].g, led.array[0].b);
		//printf("motor l %d motor r %d\n", motor_l.index, motor_r.index);
		dist_test(&dist);
		
		ws2812b_show(&led);
		
		_delay_ms(10);
    }
}

