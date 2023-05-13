#ifndef RC5_H
#define RC5_H

#include <stdint.h>

typedef uint16_t rc5_message_t;

#define RC5_MESSAGE_START(message)		(((message) & 0x3000)>>12)
#define RC5_MESSAGE_TOGGLE(message)		(((message) & 0x0800)>>11)
#define RC5_MESSAGE_ADDRESS(message)	(((message) & 0x07C0)>>6)
#define RC5_MESSAGE_COMMAND(message)	((message) & 0x3F)

void rc5_init();
uint8_t rc5_get_message(rc5_message_t *);

#endif
