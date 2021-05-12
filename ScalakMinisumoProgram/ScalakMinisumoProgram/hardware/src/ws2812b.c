/*
 * ws2812b.c
 *
 * Created: 02.05.2021 18:15:40
 *  Author: ermoz
 */ 

#include "../include/ws2812b.h"

const ColorRGB_t rainbow_colors[] PROGMEM = {
	{255, 0, 0},
	{255, 127, 0},
	{255, 255, 0},
	{127, 255, 0},
	{0, 255, 0},
	{0, 255, 127},
	{0, 255, 255},
	{0, 127, 255},
	{0, 0, 255},
	{127, 0, 255},
	{255, 0, 255},
	{255, 0, 127}
};

ColorRGB_t multiply(ColorRGB_t color, float n) {
	return (ColorRGB_t) {
		color.r * n,
		color.g * n,
		color.b * n
	};
}

ColorRGB_t interpolate(ColorRGB_t color1, ColorRGB_t color2, float alpha) {
	return (ColorRGB_t) {
		(1.-alpha)*color1.r + alpha*color2.r,
		(1.-alpha)*color1.g + alpha*color2.g,
		(1.-alpha)*color1.b + alpha*color2.b
	};
}

void ws2812b_init(WS2812B_t *strip, uint16_t n) {
	strip->n = n;
	strip->array = (ColorRGB_t*)malloc(n*sizeof(ColorRGB_t));
	strip->animation = 0;
	
	DDRB |= (1<<PINB0);
	PORTB &= ~(1<<PINB0);
}

void ws2812b_rainbow_step(WS2812B_t *strip, float dX, float T) {
	uint16_t N = (float)strip->n;
	uint16_t K = sizeof(rainbow_colors)/sizeof(ColorRGB_t);
	float X = strip->animation;
	
	for(uint16_t i=0; i<N; i++) {
		float index = (float)(X+i)/((float)T/N);

		ColorRGB_t color1, color2;
		memcpy_P(&color1, &rainbow_colors[(uint8_t)floor(index)%K], sizeof(ColorRGB_t));
		memcpy_P(&color2, &rainbow_colors[(uint8_t) ceil(index)%K], sizeof(ColorRGB_t));
		
		float alpha = index - floor(index);
		strip->array[i] = interpolate(color1, color2, alpha);
		strip->array[i] = multiply(strip->array[i], 0.15);
	}
	
	strip->animation +=dX;
}

void ws2812b_show(WS2812B_t *strip) {
	ColorRGB_t *array = strip->array;
	uint8_t n = strip->n;

	// source code:
	// https://github.com/pololu/pololu-led-strip-avr

	cli();
	while(n--) {
		// Send a color to the LED strip.
		// The assembly below also increments the 'colors' pointer,
		// it will be pointing to the next color at the end of this loop.
		asm volatile(
			"ld __tmp_reg__, %a0+\n"
			"rcall send_led_strip_byte%=\n"  // Send red component.
			"ld __tmp_reg__, %a0+\n"
			"rcall send_led_strip_byte%=\n"  // Send green component.
			"ld __tmp_reg__, %a0+\n"
			"call send_led_strip_byte%=\n"   // Send blue component.
			"rjmp led_strip_asm_end%=\n"     // Jump past the assembly subroutines.

			// send_led_strip_byte subroutine:  Sends a byte to the LED strip.
			"send_led_strip_byte%=:\n"
			"rcall send_led_strip_bit%=\n"   // Send most-significant bit (bit 7).
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
			
			: "=b" (array)
			: "0" (array),							// %a0 points to the next color to display
			"I" (_SFR_IO_ADDR(PORTB)),		// %2 is the port register (e.g. PORTC)
			"I" (PINB0)						// %3 is the pin number (0-8)
		);
	}
	sei();
	_delay_us(80);									// Send the reset signal.
	
}