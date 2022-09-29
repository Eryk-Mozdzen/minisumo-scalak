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

uint8_t led_state = 0;

void blink() {
	ws2812b_set(0, 0, 255*led_state);
	led_state = !led_state;
}

rc5_message_t message;

void check_rc5() {
	if(rc5_get_message(&message)) {
		printf("toggle: 0x%02X address: 0x%02X command: 0x%02X\n\r", message.toggle, message.address, message.command);
	}
}

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

uint8_t pwr;

void motor_test() {
	pwr++;
	motors_set(pwr, pwr);
}

ISR(TIMER0_OVF_vect) {
	scheduler_tick();
}

int main() {

	uart_init(38400);
	rc5_init();
	periph_init();
	motors_init();

	// tick generation
	// timer 0 overflow interrupt (33ms)
	// this scales down right motor PWM frequency 1024 times
	TCCR0B = (1<<CS02) | (1<<CS00);
    TIMSK0 = (1<<TOIE0);
	sei();

	scheduler_add_task(1, blink, 1000/33);
	scheduler_add_task(2, check_rc5, 0);
	scheduler_add_task(3, debug, 100/33);
	scheduler_add_task(4, motor_test, 10/33);

	scheduler_start();

}