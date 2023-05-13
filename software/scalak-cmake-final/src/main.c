#include "led.h"
#include "periph.h"
#include "motors.h"
#include "robot.h"
#include "scheduler.h"
#include <avr/interrupt.h>

#ifdef PRINT
#include "uart.h"
#endif

#ifndef EXTERNAL_MODULE
#include "rc5.h"
#endif

#ifdef PRINT
void print_sensor_info() {
	uint8_t line[2];
	uint16_t line_raw[2];

	line_get(line);
	line_get_raw(line_raw);

	printf("raw: %d %d state: %d %d\n\r", line_raw[0], line_raw[1], line[0], line[1]);

	uint8_t prox[5];

	prox_get(prox);

	printf("prox: %d%d%d%d%d\n\r", prox[0], prox[1], prox[2], prox[3], prox[4]);

	int16_t motors[2];

	motors_get(motors);

	printf("motors: %d %d\n\r", motors[0], motors[1]);

	uint8_t sw = switch_get();
	uint8_t butt = button_get();

	printf("switch: %d\n\rbutton: %d\n\r", sw, butt);
}
#endif

ISR(TIMER0_OVF_vect) {
	scheduler_tick_count++;
}

int main() {

	led_init();
	periph_init();
	motors_init();

	#ifdef PRINT
	uart_init();
	#endif

	#ifndef EXTERNAL_MODULE
	rc5_init();
	#endif

	// tick generation
	// timer 0 overflow interrupt (8ms)
	TCCR0B |=(1<<CS02);
    TIMSK0 |=(1<<TOIE0);
	sei();

	robot_init();

	#ifdef PRINT
	scheduler_add_task(print_sensor_info, 100/TICK_MS);
	#endif

	scheduler_add_task(line_task, 100/TICK_MS);
	scheduler_add_task(prox_task, 1);
	scheduler_add_task(loop_task, 1);

	scheduler_start();

	return 0;
}