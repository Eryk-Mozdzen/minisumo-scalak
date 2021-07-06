#include "ws2812b.h"

void rgb_color_set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
	for(uint8_t i=0; i<LED_STRIP_SIZE; i++)
	rgb_color_array[i] = (rgb_color){.r = red,.g =  green,.b = blue};
	led_strip_write();
}

void rgb_color_set(rgb_color color) {
	for(uint8_t i=0; i<LED_STRIP_SIZE; i++)
		rgb_color_array[i] = color;
	led_strip_write();
}

rgb_color rgb_color_multiply(rgb_color color, double number) {
	return (rgb_color){.r = color.r*number, .g = color.g*number, .b = color.b*number};
}

void __attribute__((noinline)) led_strip_write() {
	LED_STRIP_PORT &= ~(1<<LED_STRIP_PIN);
	LED_STRIP_DDR |= (1<<LED_STRIP_PIN);
	
	uint16_t count = LED_STRIP_SIZE;

	cli();
	while(count--) {
		asm volatile(
		"ld __tmp_reg__, %a0+\n"
		"ld __tmp_reg__, %a0\n"
		"rcall send_led_strip_byte%=\n"
		"ld __tmp_reg__, -%a0\n"
		"rcall send_led_strip_byte%=\n"
		"ld __tmp_reg__, %a0+\n"
		"ld __tmp_reg__, %a0+\n"
		"ld __tmp_reg__, %a0+\n"
		"rcall send_led_strip_byte%=\n"
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
		#if F_CPU == 8000000
		"rol __tmp_reg__\n"
		#endif
		"sbi %2, %3\n"

		#if F_CPU != 8000000
		"rol __tmp_reg__\n"
		#endif

		#if F_CPU == 16000000
		"nop\n" "nop\n"
		#elif F_CPU == 20000000
		"nop\n" "nop\n" "nop\n" "nop\n"
		#elif F_CPU != 8000000
		#error "Unsupported F_CPU"
		#endif

		"brcs .+2\n" "cbi %2, %3\n"

		#if F_CPU == 8000000
		"nop\n" "nop\n"
		#elif F_CPU == 16000000
		"nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
		#elif F_CPU == 20000000
		"nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
		"nop\n" "nop\n"
		#endif

		"brcc .+2\n" "cbi %2, %3\n"

		"ret\n"
		"led_strip_asm_end%=: "
		: "=b" (rgb_color_array)
		: "0" (rgb_color_array),
		"I" (_SFR_IO_ADDR(LED_STRIP_PORT)),
		"I" (LED_STRIP_PIN)
		);
	}
	sei();
	_delay_us(80);
}