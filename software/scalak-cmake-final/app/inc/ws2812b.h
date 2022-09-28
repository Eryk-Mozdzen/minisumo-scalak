#ifndef WS2812B_H_
#define WS2812B_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_STRIP_PORT	PORTB
#define LED_STRIP_DDR	DDRB
#define LED_STRIP_PIN	0

#define LED_STRIP_SIZE	1

typedef struct rgb_color {
	uint8_t r, g, b;
} rgb_color;

rgb_color rgb_color_array[LED_STRIP_SIZE];

void rgb_color_set_rgb(uint8_t, uint8_t, uint8_t);

void rgb_color_set(rgb_color);

rgb_color rgb_color_multiply(rgb_color, float);

void __attribute__((noinline)) led_strip_write();

#endif