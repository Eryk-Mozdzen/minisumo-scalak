#ifndef UART_H
#define UART_H

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char buffer[64];
char str[64];
int bufferLen;

void uart_write(unsigned char);
unsigned char uart_read();
uint8_t uart_equal(char*, char*);
int uart_size(char*);
char* uart_intToString(int);
int uart_stringToInt(char*);

void uart_begin(unsigned long);
void uart_print(char*);
void uart_print_int(int);
void uart_print_double(double, int);
void uart_println(char*);
void uart_println_int(int);
uint8_t uart_listen();
char* uart_getString();

void uart_printf(char*, ...);

#endif