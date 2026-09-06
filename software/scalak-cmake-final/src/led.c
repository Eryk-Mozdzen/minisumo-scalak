#include "led.h"
#include <avr/interrupt.h>

static struct {
	uint8_t r, g, b;
} color[1];

void led_init() {
	DDRB |=(1<<PINB0);
	PORTB &=~(1<<PINB0);

	led_set(0, 0, 0);
}

void led_set(uint8_t red, uint8_t green, uint8_t blue) {

	color[0].r = red;
	color[0].g = green;
	color[0].b = blue;

	cli();

	asm volatile(
		"ld __tmp_reg__, %a0+\n"
		"rcall send_led_strip_byte%=\n"
		"ld __tmp_reg__, %a0+\n"
		"rcall send_led_strip_byte%=\n"
		"ld __tmp_reg__, %a0+\n"
		"call send_led_strip_byte%=\n"
		"rjmp led_strip_asm_end%=\n"

		"send_led_strip_byte%=:\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"rcall send_led_strip_bit%=\n"
		"ret\n"

		"send_led_strip_bit%=:\n"
		"rol __tmp_reg__\n"
		"sbi %2, %3\n"
		"brcs .+2\n"
		"cbi %2, %3\n"
		"nop\n" "nop\n"
		"brcc .+2\n"
		"cbi %2, %3\n"
		"ret\n"

		"led_strip_asm_end%=: "

		: "=b" (color)
		: "0" (color),
		"I" (_SFR_IO_ADDR(PORTB)),
		"I" (0)
	);

	sei();
}
