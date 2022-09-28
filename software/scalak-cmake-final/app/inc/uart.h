#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <avr/io.h>

void uart_init(unsigned long);
int uart_putchar(char, FILE*);
int uart_getchar(FILE*);

// AtmelStudio floating point configuration setup
// http://mirekk36.blogspot.com/2013/04/eclipse-nie-dziaa-mi-printf-dla-float.html

#endif