#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <scheduler.h>
#include <finite_state_machine.h>

#include "ws2812b.h"
#include "rc5.h"
#include "uart.h"

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

ISR(TIMER0_OVF_vect) {
	scheduler_tick();
}

int main() {

	uart_init(38400);
	rc5_init();

	// tick generation
	// timer 0 overflow interrupt (33ms)
	TCCR0B = (1<<CS02) | (1<<CS00);
    TIMSK0 = (1<<TOIE0);
	sei();

	scheduler_add_task(1, blink, 1000/33);
	scheduler_add_task(2, check_rc5, 0);

	scheduler_start();

}