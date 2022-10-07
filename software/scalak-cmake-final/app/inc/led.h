#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_STRIP_PORT	PORTB
#define LED_STRIP_DDR	DDRB
#define LED_STRIP_PIN	0

#define LED_STRIP_SIZE	1

void led_set(uint8_t, uint8_t, uint8_t);

#endif