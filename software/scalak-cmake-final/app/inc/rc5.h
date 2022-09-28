#ifndef RC5_H_
#define RC5_H_

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SHORT_MIN	2*444
#define SHORT_MAX	2*1333
#define LONG_MIN	2*1334
#define LONG_MAX	2*2222

#define STATE_START1	0
#define STATE_MID1		1
#define STATE_MID0		2
#define STATE_START0	3
#define STATE_ERROR		4
#define STATE_BEGIN		5
#define STATE_END		6

volatile bool decoder_rc5_isNew;
volatile uint16_t decoder_rc5_message;
int decoder_rc5_currentState;
int decoder_rc5_counter;
uint16_t decoder_rc5_delayFull;

void decoder_rc5_initialize();
void decoder_rc5_reset();
void decoder_rc5_INT_ISR();
void decoder_rc5_TIMER_ISR();

bool decoder_rc5_new_message();
uint8_t decoder_rc5_get_toggle();
uint8_t decoder_rc5_get_address();
uint8_t decoder_rc5_get_command();

#endif