/*
 * dist.h
 *
 * Created: 12.05.2021 23:57:06
 *  Author: ermoz
 */ 


#ifndef DIST_H_
#define DIST_H_

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define DIST_L		0x01
#define DIST_LF		0x02
#define DIST_F		0x04
#define DIST_RF		0x08
#define DIST_R		0x10

#define DIST_NONE	0x00
#define DIST_ALL	0x1F

typedef struct {
	uint8_t active;
} Dist_t;

void dist_init(Dist_t*, uint8_t);
void dist_test(Dist_t*);
uint8_t dist_get_state(Dist_t*, uint8_t);

#endif /* DIST_H_ */