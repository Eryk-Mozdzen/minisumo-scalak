#include <avr/io.h>
#include <math.h>
#include <stdbool.h>
#include "uart.h"

void uart_begin(unsigned long baudRate) {
	//unsigned long baudRateValue = 103;
	unsigned long baudRateValue = (8000000/(16*baudRate))-1;
	UBRR0L = baudRateValue;
	UBRR0H = (baudRateValue>>8);
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C |= 1<<UPM00;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	bufferLen = 0;
	for(int i=0; i<64; i++)
	buffer[i] = '#';
	buffer[63] = '\0';
	
	for(int i=0; i<64; i++)
	str[i] = '#';
	str[63] = '\0';
}

void uart_write(unsigned char data) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char uart_read() {
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

uint8_t uart_listen() {
	bool newS = false;
	if(UCSR0A & (1<<RXC0)) {
		char c = uart_read();
		if(c!='\n' && c!='\0') {
			buffer[bufferLen] = c;
			bufferLen++;
		}
		
		if(c=='\n') {
			for(int i=0; i<64; i++) {
				if(i<bufferLen) str[i] = buffer[i];
				else if(i==bufferLen) str[i] = '\0';
				else str[i] = 0;
			}
			
			for(int i=0; i<64; i++)
			buffer[i] = '#';
			buffer[63] = '\0';
			bufferLen = 0;
			
			newS = true;
		}
	}
	return newS;
}

void uart_print(char* str){
	int index = 0;
	while(str[index]!='\0') {
		uart_write((unsigned char)str[index]);
		index++;
	}
}

void uart_print_int(int number) {
	char* str = uart_intToString(number);
	uart_print(str);
}

void uart_println(char* str){
	uart_print(str);
	uart_print("\n\0");
}

void uart_println_int(int number){
	uart_print_int(number);
	uart_print("\n\0");
}

char* uart_getString() {
	return str;
}

char* uart_intToString(int number) {
	static char str[20];
	int index = 0;
	
	if(number<0) {
		str[0] = '-';
		index++;
		number *=-1;
	}
	
	int digits = 1;
	if(number>0) digits +=(int)log10((double)number);
	
	for(int i=digits-1; i>=0; i--) {
		int n = (int)(number/pow(10, i));
		str[index] = (char)(n+'0');
		index++;
		number -=n*pow(10, i);
		str[index] = '\0';
	}
	
	for(int i=index+1; i<20; i++)
		str[i] = '\0';
	
	return str;
}

int uart_stringToInt(char* str) {
	int number = 0;
	int sign = 1;
	int index = 0;
	
	if(str[index]=='-') {
		sign = -1;
		index++;
	}
	
	while(str[index]!='\0') {
		number *=10;
		number +=(int)(str[index]-'0');
		index++;
	}
	
	return number*sign;
}

uint8_t uart_equal(char* str1, char* str2) {
	int index = 0;
	while(true) {
		if(str1[index]!=str2[index]) return false;
		if(str1[index]=='\0') return true;
		if(str2[index]=='\0') return true;
		index++;
	}
}

int uart_size(char* str) {
	int number = 0;
	while(str[number]!='\0')
		number++;
	return number;
}

void uart_print_double(const double number, int precision) {
	static char arr[20];
	dtostrf(number, sizeof number, precision, arr);
	uart_print(arr);
}

void uart_printf(char* layout, ...) {
	int number = 0;
	for(int i=0; i<uart_size(layout)-1; i++)
		if(layout[i]=='%' && layout[i+1]!='%') number++;

	va_list arguments;
	va_start(arguments, number);

	for(int i=0; i<uart_size(layout); i++) {
		if(layout[i]=='%') {
			i++;
			switch(layout[i]) {
				case 'i': uart_print_int((int)va_arg(arguments, int)); break;
				case 'd': uart_print_double((double)va_arg(arguments, double), 2); break;
				case 's': uart_print((char*)va_arg(arguments, char*)); break;
				case '%': uart_print("%"); break;
				case 'b': if((bool)va_arg(arguments, int)) uart_print("true "); else uart_print("false"); break;
				case 'c': {
					char* message = "#\0";
					message[0] = (char)va_arg(arguments, int);
					uart_print(message); break;
				}
			}
		} else {
			char* message = "#";
			message[0] = layout[i]; 
			uart_print(message);
		}
	}
}