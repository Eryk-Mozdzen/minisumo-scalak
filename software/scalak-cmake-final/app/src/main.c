#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <scheduler.h>
#include <finite_state_machine.h>

#include "ws2812b.h"
#include "rc5.h"
#include "uart.h"
#include "periph.h"
#include "motors.h"

#include "robot.h"

void debug() {
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

ISR(TIMER0_OVF_vect) {
	scheduler_tick();
}

int main() {

	uart_init(38400);
	rc5_init();
	periph_init();
	motors_init();

	ws2812b_set(0, 0, 0);

	// tick generation
	// timer 0 overflow interrupt (33ms)
	// this scales down right motor PWM frequency 1024 times
	TCCR0B = (1<<CS02) | (1<<CS00);
    TIMSK0 = (1<<TOIE0);
	sei();

	//scheduler_add_task(debug, 100/33);

	robot_init();

	scheduler_start();

}