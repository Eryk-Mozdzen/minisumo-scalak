#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

void motors_init();
void motors_limit(uint8_t);
void motors_set(int16_t, int16_t);
void motors_get(int16_t *);

#endif
