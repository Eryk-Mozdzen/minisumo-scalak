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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware_config.h"

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorRGB_t;

typedef struct {
	ColorRGB_t* array;
	uint16_t n;
} WS2812B_t;

void ws2812b_init(WS2812B_t*, uint16_t);
void ws2812b_show(WS2812B_t*);
//void ws2812b_show(ColorRGB*);

#endif /* LED_WS2812B_H_ */