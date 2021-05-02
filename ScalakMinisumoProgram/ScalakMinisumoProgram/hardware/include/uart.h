/*
 * uart.h
 *
 * Created: 02.05.2021 16:48:16
 *  Author: ermoz
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

void uart_init(unsigned long);
int uart_putchar(char, FILE*);
int uart_getchar(FILE*);

// AtmelStudio floating point configuration setup
// http://mirekk36.blogspot.com/2013/04/eclipse-nie-dziaa-mi-printf-dla-float.html

#endif /* UART_H_ */