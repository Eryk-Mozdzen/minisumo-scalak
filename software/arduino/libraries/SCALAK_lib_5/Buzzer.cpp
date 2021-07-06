#include "SCALAK_lib_5.h"

Buzzer::Buzzer() {}

Buzzer::Buzzer(int pin_a) {
    pin = pin_a;
}

void Buzzer::initialize() {
    pinMode(pin,OUTPUT);
    Buzzer::stop();
}

void Buzzer::play() {
    digitalWrite(pin,HIGH);
}

void Buzzer::stop() {
    digitalWrite(pin,LOW);
}