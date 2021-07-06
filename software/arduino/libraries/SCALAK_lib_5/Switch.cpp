#include "SCALAK_lib_5.h"

Switch::Switch() {}

Switch::Switch(int pin_a) {
	pin = pin_a;
}

void Switch::initialize() {
	pinMode(pin,INPUT_PULLUP);
}

bool Switch::getState() {
	if(pin>=A0 && pin<=A7) return (!(analogRead(pin)>500));
	return !digitalRead(pin);
}

void Switch::print() {
	Serial.println(Switch::getState());
}