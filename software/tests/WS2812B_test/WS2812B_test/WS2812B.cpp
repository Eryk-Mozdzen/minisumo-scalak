#include "WS2812B.h"

void __attribute__((noinline)) Leds::led_strip_write(rgb_color* colors, unsigned int count) {
	LED_STRIP_PORT &= ~(1<<LED_STRIP_PIN);
	LED_STRIP_DDR |= (1<<LED_STRIP_PIN);

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
		: "=b" (colors)
		: "0" (colors),
		"I" (_SFR_IO_ADDR(LED_STRIP_PORT)),
		"I" (LED_STRIP_PIN)
		);
	}
	sei();
	_delay_us(80);
}