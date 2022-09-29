#ifndef MOTORS_H
#define MOTORS_H

#include <avr/io.h>
#include <scheduler.h>

void motors_init();
void motors_set(int16_t, int16_t);
void motors_get(int16_t *);

#endif
