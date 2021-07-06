#include "SCALAK_lib_4.h"

QTR1A::QTR1A() {}

QTR1A::QTR1A(int a,int b) {
	sensor[0] = a;
	sensor[1] = b;
	inv = true;
}

void QTR1A::initialize(int mode) {
	for(int i=0; i<2; i++)
		pinMode(sensor[i],INPUT);
	QTR1A::setMode(mode);
}

bool QTR1A::getStateRAW(int index) {
	if(inv) return digitalRead(sensor[index]);
	else return !digitalRead(sensor[index]);
}

bool QTR1A::getState(int index) {
	int good = 0;
	for(int i=0; i<10; i++)
		if(QTR1A::getStateRAW(index))
			good++;
	return (good>=7);
}

void QTR1A::setMode(int mode) {
	if(mode==0) inv = true;
	else if(mode==1) inv = false;
	else if(mode==2) {
		bool left = QTR1A::getState(LEFT);
		bool right = QTR1A::getState(RIGHT);
		if(left==right) inv = !left;
	}
}

int QTR1A::getHorizon() {
	return 2*QTR1A::getState(LEFT) + QTR1A::getState(RIGHT);
}

void QTR1A::print() {
	Serial.print(QTR1A::getState(LEFT));
	Serial.print(QTR1A::getState(RIGHT));
	Serial.print('\n');
}