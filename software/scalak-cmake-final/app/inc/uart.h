#ifndef UART_H
#define UART_H

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define UART_BAUDRATE	38400

void uart_init();

// AtmelStudio floating point configuration setup
// http://mirekk36.blogspot.com/2013/04/eclipse-nie-dziaa-mi-printf-dla-float.html

#endif