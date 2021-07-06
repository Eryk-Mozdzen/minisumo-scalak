#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

Stream Serial;

int main() {
	
	Serial.begin(9600);
	
    while(true) {
		bool isNew = Serial.listen();
		
		char* str = Serial.getString();
		
		if(isNew) {
			Serial.print("Type:\t\0");
			Serial.println(Stream::getCommandType(str));
			Serial.print("Index:\t\0");
			Serial.println(Stream::getCommandIndex(str));
			Serial.print("Value:\t\0");
			Serial.println(Stream::getCommandValue(str));
		}
		
		//Serial.println(Stream::createCommand("MOTO\0", 1, -150));
    }
	
	return 0;
}

