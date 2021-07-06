#include "SCALAK_lib_4.h"

IR38kHz::IR38kHz() {}

IR38kHz::IR38kHz(int a,int b,int c,int d,int e) {
	sensor[0] = a;
	sensor[1] = b;
	sensor[2] = c;
	sensor[3] = d;
	sensor[4] = e;
}

void IR38kHz::initialize() {
	for(int i=0; i<5; i++)
		pinMode(sensor[i],INPUT);
}

bool IR38kHz::getStateRAW(int index) {
    if(sensor[index]>=A0 && sensor[index]<=A7) return (analogRead(sensor[index])<500);
	else return !digitalRead(sensor[index]);
}

bool IR38kHz::getState(int index) {
    int good = 0;
	for(int i=0; i<20; i++)
		if(IR38kHz::getStateRAW(index))
			good++;
	//return (good>=15);
	return IR38kHz::getStateRAW(index);
}

float IR38kHz::getError(int mode) {
	if(mode==0) return IR38kHz::getErrorSingle();
	if(mode==1) return IR38kHz::getErrorMulti();
}

float IR38kHz::getErrorSingle() {
	int active = 0;
	int sum = 0;

	for(int i=0; i<5; i++) {
		if(IR38kHz::getState(i)) {
			active++;
			sum +=(i-2);
		}
	}

	return (float)(sum)/(float)(active);
}

float IR38kHz::getErrorMulti() {
	int active = 0;
	int sum = 0;
	float smallest = 1000;

	for(int i=0; i<5; i++) {
		if(IR38kHz::getState(i)) {
			active++;
			sum +=(i-2);
		}
		if((!IR38kHz::getState(i) && active!=0) || i==4) {
			float error = (float)(sum)/(float)(active);
			if(abs(error)<abs(smallest))
				smallest = error;
			active = 0;
			sum = 0;
		}
	}

	if(smallest!=1000) return smallest;
	else return 0.0/0.0;
}

void IR38kHz::print() {
	for(int i=0; i<5; i++)
		Serial.print(IR38kHz::getState(i));
	Serial.print('\n');
}