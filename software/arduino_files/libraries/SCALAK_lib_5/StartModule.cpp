#include "SCALAK_lib_5.h"

StartModule::StartModule() {}

StartModule::StartModule(int startPin_a) {
	startPin = startPin_a;
	mode = OFF;
	debug = false;
	click = false;
	clicked = false;
}

void StartModule::initialize(int mode) {
	pinMode(startPin,INPUT_PULLUP);
	setMode(mode);
}

void StartModule::wait(void(*func)(), void(*quitFunc)()) {

	switch(mode) {
		case OFF: break;
		case MODULE:
			while(StartModule::read()) {
				func();
				if(debug)
					Serial.println("Module: Stopped");
				delay(10);

				if(!StartModule::read())
					quitFunc();
			} break;
		case MANUAL:
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
					Serial.println("Manual: Stopped");
				delay(10);

				if(click)
					quitFunc();
			} break;
	}

	if(debug) {
		switch(mode) {
			case OFF: Serial.println("Off"); break;
			case MODULE: Serial.println("Module: Running"); break;
			case MANUAL: Serial.println("Manual: Running"); break;
		}
	}
}

void StartModule::setMode(int mode_a) {
	mode = mode_a;

	if(mode==AUTO) {
		if(StartModule::read()) mode = MODULE;
		else mode = MANUAL;
	}
}

void StartModule::print() {
	debug = true;
}

bool StartModule::read() {
	return !digitalRead(startPin);
}

int StartModule::getMode() {
	return mode;
}