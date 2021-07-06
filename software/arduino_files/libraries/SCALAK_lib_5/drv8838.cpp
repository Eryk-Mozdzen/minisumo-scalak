#include "SCALAK_lib_5.h"

drv8838::drv8838() {};

drv8838::drv8838(int ENABLE1,int PHASE1,int ENABLE2,int PHASE2) {
	ENABLEpin[0] = ENABLE1;
	PHASEpin[0] = PHASE1;
	ENABLEpin[1] = ENABLE2;
	PHASEpin[1] = PHASE2;

	for(int i=0; i<2; i++) {
		rotate[i] = true;
		power[i] = 0;
	}
}

void drv8838::initialize(int flip1, int flip2) {
	for(int i=0; i<2; i++) {
		pinMode(ENABLEpin[i], OUTPUT);
		pinMode(PHASEpin[i], OUTPUT);
	}

	if(flip1==BACKWARD) drv8838::flip(LEFT);
	if(flip2==BACKWARD) drv8838::flip(RIGHT);
}

void drv8838::setSpeeds(int speed1,int speed2) {
	drv8838::setSpeed(LEFT,speed1);
	drv8838::setSpeed(RIGHT,speed2);
}

void drv8838::setSpeeds(int speed) {
	drv8838::setSpeed(LEFT,speed);
	drv8838::setSpeed(RIGHT,speed);
}

void drv8838::flip(int index) {
	rotate[index] = !rotate[index];
}

void drv8838::print() {
	for(int i=0; i<2; i++) {
		Serial.print(power[i]);
		Serial.print('\t');
	}
	Serial.print('\n');
}

void drv8838::stop() {
	drv8838::setSpeed(LEFT,0);
	drv8838::setSpeed(RIGHT,0);
}

void drv8838::stop(int index) {
	drv8838::setSpeed(index,0);
}

int drv8838::getSpeed(int index) {
	return power[index];
}

int drv8838::fix(int input) {
	if(input>255) input = 255;
	if(input<-255) input = -255;
	return input;
}

void drv8838::setSpeed(int index, int speed) {
	power[index] = drv8838::fix(speed);

	analogWrite(ENABLEpin[index], abs(power[index]));
	if(power[index]!=0)
		digitalWrite(PHASEpin[index], (rotate[index] ^ power[index]>0));
}