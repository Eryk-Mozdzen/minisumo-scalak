#include <avr/io.h>
#include <math.h>
#include "UART.h"

Stream::Stream() {}

void Stream::begin(unsigned long baudRate) {
	//unsigned long baudRateValue = 103;
	unsigned long baudRateValue = (16000000/(16*baudRate))-1;
	UBRRL = baudRateValue;
	UBRRH = (baudRateValue>>8);
	
	UCSRA = 0x00;
	UCSRB = (1<<TXEN)|(1<<RXEN);
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	
	bufferLen = 0;
	for(int i=0; i<64; i++)
		buffer[i] = '#';
	buffer[63] = '\0';
	
	for(int i=0; i<64; i++)
		str[i] = '#';
	str[63] = '\0';
}

void Stream::write(unsigned char data) {
	while(!(UCSRA&(1<<UDRE)));
	UDR = data;
	while(!(UCSRA&(1<<TXC)));
}

unsigned char Stream::read() {
	while(!(UCSRA&(1<<RXC)));
	return UDR;
}

bool Stream::listen() {
	bool newS = false;
	if(UCSRA&(1<<RXC)) {
		char c = Stream::read();
		if(c!='\n' && c!='\0') {
			buffer[bufferLen] = c;
			bufferLen++;
		}
		
		if(c=='\r') {
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

void Stream::print(char* str){
	int index = 0;
	while(str[index]!='\0') {
		Stream::write((unsigned char)str[index]);
		index++;
	}
}

void Stream::print(int number) {
	char* str = Stream::intToString(number);
	Stream::print(str);
}

void Stream::println(char* str){
	Stream::print(str);
	Stream::print("\r\n\0");
}

void Stream::println(int number){
	Stream::print(number);
	Stream::print("\r\n\0");
}

char* Stream::getString() {
	return str;
}

char* Stream::intToString(int number) {
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
	}
	
	str[index] = '\0';
	
	for(int i=index+1; i<20; i++)
		str[i] = 0;
	
	return str;	
}

int Stream::stringToInt(char* str) {
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

char* Stream::getCommandType(char* command) {
	static char str[] = "####\0";
	for(int i=0; i<4; i++)
		str[i] = command[i];
	return str;
}

int Stream::getCommandIndex(char* command) {
	return (int)(command[4]-'0');
}

int Stream::getCommandValue(char* command) {
	static char str[20] = "##################\0";
	int index = 0;
	
	while((command[index+6]>='0' && command[index+6]<='9') || command[index+6]=='-') {
		str[index] = command[index+6];
		index++;
	}
	str[index] = '\0';
	for(int i=index+1; i<20; i++)
		str[i] = 0;
	return Stream::stringToInt(str);
}

char* Stream::createCommand(char* type, int index, int value) {
	static char str[20] = "##################\0";
	char* valueStr = Stream::intToString(value);
	
	for(int i=0; i<20; i++) {
		if(i<4) str[i] = type[i];
		else if(i==4) str[i] = (char)(index+'0');
		else if(i==5) str[i] = ' ';
		else str[i] = valueStr[i-6];
	}
	
	return str;
}