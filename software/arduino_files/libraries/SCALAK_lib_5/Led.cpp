#include "SCALAK_lib_5.h"

Led::Led() {}

Led::Led(int pin_a) {
    pin = pin_a;
    lastT = millis();
}

void Led::initialize() {
    pinMode(pin,OUTPUT);
    Led::setState(OFF);
}

void Led::blink(float frequancy) {
    float peroid = 1000.0/frequancy;
    if((lastT+(peroid)/2.0)<=millis()) {
        digitalWrite(pin, !digitalRead(pin));
        lastT = millis();
    }
}

void Led::setState(int state) {
    digitalWrite(pin, (state==ON));
}