/*
 * led_ws2812b.h
 *
 * Created: 02.05.2021 18:15:15
 *  Author: ermoz
 */ 

#ifndef WS2812B_H_
#define WS2812B_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorRGB_t;

typedef struct {
	ColorRGB_t* array;
	uint16_t n;
	float animation;
} WS2812B_t;

ColorRGB_t multiply(ColorRGB_t, float);
ColorRGB_t interpolate(ColorRGB_t, ColorRGB_t, float);

void ws2812b_init(WS2812B_t*, uint16_t);
void ws2812b_rainbow_step(WS2812B_t*, float, float);
void ws2812b_show(WS2812B_t*);

#endif /* LED_WS2812B_H_ */