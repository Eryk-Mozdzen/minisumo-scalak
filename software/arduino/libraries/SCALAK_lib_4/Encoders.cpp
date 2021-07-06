#include "SCALAK_lib_4.h"

Encoders::Encoders() {}

Encoders::Encoders(int INTERRUP1, int OTHERpin1, int INTERRUP2, int OTHERpin2) {
    INTERRUPpin[0] = INTERRUP1;
	OTHERpin[0] = OTHERpin1;
	INTERRUPpin[1] = INTERRUP2;
	OTHERpin[1] = OTHERpin2;

    for(int i=0; i<2; i++)
		counter[i] = 0;
}

void Encoders::initialize(void(*refreshCounter0_a)(), void(*refreshCounter1_a)()) {
	for(int i=0; i<2; i++) {
		pinMode(INTERRUPpin[i], INPUT);
		pinMode(OTHERpin[i], INPUT);
	}
	attachInterrupt(digitalPinToInterrupt(INTERRUPpin[0]), refreshCounter0_a, CHANGE);
	attachInterrupt(digitalPinToInterrupt(INTERRUPpin[1]), refreshCounter1_a, CHANGE);
}

int Encoders::getPosition(int index) {
    return counter[index];
}

void Encoders::reset() {
    Encoders::reset(0);
    Encoders::reset(1);
}

void Encoders::reset(int index) {
    counter[index] = 0;
}

void Encoders::print() {
    Serial.print(counter[0]);
    Serial.print('\t');
    Serial.println(counter[1]);
}

void Encoders::refreshCounter0() {
	counter[0] -=((digitalRead(INTERRUPpin[0])==digitalRead(OTHERpin[0]))*2 - 1);
}

void Encoders::refreshCounter1() {
	counter[1] +=((digitalRead(INTERRUPpin[1])==digitalRead(OTHERpin[1]))*2 - 1);
}