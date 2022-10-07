#include "led.h"

typedef struct {
	uint8_t r, g, b;
} rgb_color_t;

static rgb_color_t rgb_color_array[LED_STRIP_SIZE];

static void __attribute__((noinline)) led_strip_write() {
	LED_STRIP_PORT &=~(1<<LED_STRIP_PIN);
	LED_STRIP_DDR |=(1<<LED_STRIP_PIN);
	
	uint16_t count = LED_STRIP_SIZE;

	cli();   // Disable interrupts temporarily because we don't want our pulse timing to be messed up.
	while(count--) {
		// Send a color to the LED strip.
		// The assembly below also increments the 'colors' pointer,
		// it will be pointing to the next color at the end of this loop.
		asm volatile(
			"ld __tmp_reg__, %a0+\n"
			"rcall send_led_strip_byte%=\n"  // Send red component.
			"ld __tmp_reg__, %a0+\n"
			"rcall send_led_strip_byte%=\n"  // Send green component.
			"ld __tmp_reg__, %a0+\n"
			"call send_led_strip_byte%=\n"  // Send blue component.
			"rjmp led_strip_asm_end%=\n"    // Jump past the assembly subroutines.

			// send_led_strip_byte subroutine:  Sends a byte to the LED strip.
			"send_led_strip_byte%=:\n"
			"rcall send_led_strip_bit%=\n"  // Send most-significant bit (bit 7).
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"
			"rcall send_led_strip_bit%=\n"  // Send least-significant bit (bit 0).
			"ret\n"

			// send_led_strip_bit subroutine:  Sends single bit to the LED strip by driving the data line
			// high for some time.  The amount of time the line is high depends on whether the bit is 0 or 1,
			// but this function always takes the same time (2 us).
			"send_led_strip_bit%=:\n"
			"rol __tmp_reg__\n"                      // Rotate left through carry.
			"sbi %2, %3\n"                           // Drive the line high.
			"brcs .+2\n"						     // If the bit to send is 0
			"cbi %2, %3\n"							 // drive the line low now.
			"nop\n" "nop\n"
			"brcc .+2\n"							 // If the bit to send is 1
			"cbi %2, %3\n"							 // drive the line low now.
			"ret\n"
					
			"led_strip_asm_end%=: "
		
			: "=b" (rgb_color_array)
			: "0" (rgb_color_array),         // %a0 points to the next color to display
			"I" (_SFR_IO_ADDR(LED_STRIP_PORT)),   // %2 is the port register (e.g. PORTC)
			"I" (LED_STRIP_PIN)     // %3 is the pin number (0-8)
		);

		// Uncomment the line below to temporarily enable interrupts between each color.
		//sei(); asm volatile("nop\n"); cli();
	}
	sei();          // Re-enable interrupts now that we are done.
	
	_delay_us(80);  // Send the reset signal.
}

void led_set(uint8_t red, uint8_t green, uint8_t blue) {

	for(uint8_t i=0; i<LED_STRIP_SIZE; i++) {
		rgb_color_array[i].r = red;
		rgb_color_array[i].g = green;
		rgb_color_array[i].b = blue;
	}
		
	led_strip_write();
}
