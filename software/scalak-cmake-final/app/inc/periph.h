#ifndef PERIPH_H
#define PERIPH_H

#include <avr/io.h>
#include <scheduler.h>

#define LINE_THRESHOLD	600
#define PROX_THRESHOLD	8

void periph_init();

void line_get_raw(uint16_t *);
void line_get(uint8_t *);
void prox_get(uint8_t *);

uint8_t switch_get();
uint8_t button_get();

uint8_t eeprom_read(uint16_t);
void eeprom_write(uint16_t, uint8_t);

#endif
