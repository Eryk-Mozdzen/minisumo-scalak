#include "SCALAK_lib_4.h"

StartModule::StartModule() {}

StartModule::StartModule(int startPin_a) {
	startPin = startPin_a;
	active = true;
	debug = false;
	click = false;
	clicked = false;
}

void StartModule::initialize(int mode) {
	pinMode(startPin,INPUT_PULLUP);
	setMode(mode);
}

void StartModule::wait(void(*func)(), void(*quitFunc)(), int manualDelay) {
	if(active) {
		//Module
		while(StartModule::read()) {
			func();
			if(debug)
				Serial.println("Module: Stopped");
			delay(10);

			if(!StartModule::read())
				quitFunc();
		}
	} else {
		//Manual
		if(!StartModule::read()) clicked = false;
		if(StartModule::read() && !clicked) {
			clicked = true;
			click = !click;
		}
		while(!click) {
			func();

			if(!StartModule::read()) clicked = false;
			if(StartModule::read() && !clicked) {
				clicked = true;
				click = !click;
			}

			if(debug)
				Serial.println("Manual Start: Stopped");
			delay(10);

			if(click) {
				quitFunc();
				delay(manualDelay);
			}
		}
	}

	if(debug) {
		if(active) Serial.println("Module: Running");
		else if(click) Serial.println("Manual Start: Running");
	}
}

void StartModule::setMode(int mode) {
	if(mode==MODULE) active = true;
	if(mode==MANUAL) active = false;
	if(mode==AUTO) active = !digitalRead(startPin);
}

void StartModule::print() {
	debug = true;
}

bool StartModule::read() {
	return !digitalRead(startPin);
}