#include "uart.h"

void uart_init(unsigned long baud_rate) {
	unsigned long baud_rate_value = (F_CPU/(16*baud_rate))-1;
	UBRR0L = baud_rate_value;
	UBRR0H = (baud_rate_value>>8);

	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= 1<<UPM00;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);

	fdevopen(uart_putchar, uart_getchar);
}

int uart_putchar(char data, FILE *stream) {
	(void)stream;

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	return data;
}

int uart_getchar(FILE *stream) {
	(void)stream;

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}