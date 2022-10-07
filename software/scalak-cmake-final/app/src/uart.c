#include "uart.h"

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
	uint16_t baud_rate_value = (F_CPU/(16*UART_BAUDRATE))-1;
	UBRR0L = baud_rate_value & 0x00FF;
	UBRR0H = ((baud_rate_value & 0xFF00)>>8);

	UCSR0B |=(1<<RXEN0) | (1<<TXEN0);
	UCSR0C |=(1<<UPM00) | (1<<UCSZ01) | (1<<UCSZ00);

	fdevopen(uart_putchar, uart_getchar);
}
