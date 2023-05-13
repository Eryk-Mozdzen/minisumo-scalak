#include "uart.h"
#include <avr/io.h>

#define UART_BAUDRATE	38400

static int uart_putchar(char data, FILE *stream) {
	(void)stream;

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	return data;
}

static int uart_getchar(FILE *stream) {
	(void)stream;

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void uart_init() {
	UBRR0 = (F_CPU/(16*UART_BAUDRATE)) - 1;

	UCSR0B |=(1<<RXEN0) | (1<<TXEN0);
	UCSR0C |=(1<<UPM00) | (1<<UCSZ01) | (1<<UCSZ00);

	fdevopen(uart_putchar, uart_getchar);
}
