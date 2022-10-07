#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <avr/interrupt.h>

void led_init();
void led_set(uint8_t, uint8_t, uint8_t);

#endif