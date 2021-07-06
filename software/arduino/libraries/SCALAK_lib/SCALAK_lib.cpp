// SCALAK_lib.cpp
// Eryk Mozdzen 2019

#include "SCALAK_lib.h"

drv8838::drv8838() {};

drv8838::drv8838(int ENABLE1,int PHASE1,int ENABLE2,int PHASE2) {
	ENABLEpin1 = ENABLE1;
	PHASEpin1 = PHASE1;
	ENABLEpin2 = ENABLE2;
	PHASEpin2 = PHASE2;
	pinMode(ENABLEpin1, OUTPUT);
	pinMode(PHASEpin1, OUTPUT);
	pinMode(ENABLEpin2, OUTPUT);
	pinMode(PHASEpin2, OUTPUT);
	rotate1 = true;
	rotate2 = true;
	speedA = 0;
	speedB = 0;
}

void drv8838::setSpeeds(int speed1,int speed2) {
    if(speed1>0) {
        analogWrite(ENABLEpin1,speed1);
        digitalWrite(PHASEpin1,rotate1);
    } else {
        analogWrite(ENABLEpin1,-speed1);
        digitalWrite(PHASEpin1,!rotate1);
    }
    if(speed2>0) {
        analogWrite(ENABLEpin2,speed2);
        digitalWrite(PHASEpin2,rotate2);
    } else {
        analogWrite(ENABLEpin2,-speed2);
        digitalWrite(PHASEpin2,!rotate2);
    }
    rotate1 ? speedA = speed1 : speedA = -speed1;
    rotate2 ? speedB = speed2 : speedB = -speed2;
}

void drv8838::setSpeeds(int speed) {
    if(speed>0) {
        analogWrite(ENABLEpin1,speed);
        analogWrite(ENABLEpin2,speed);
        digitalWrite(PHASEpin1,rotate1);
        digitalWrite(PHASEpin2,rotate2);
    } else {
        analogWrite(ENABLEpin1,-speed);
        analogWrite(ENABLEpin2,-speed);
        digitalWrite(PHASEpin1,!rotate1);
        digitalWrite(PHASEpin2,!rotate2);
    }
    rotate1 ? speedA = speed : speedA = -speed;
    rotate2 ? speedB = speed : speedB = -speed;
}

void drv8838::stop() {
    speedA = 0;
    speedB = 0;
    analogWrite(ENABLEpin1,0);
    analogWrite(ENABLEpin2,0);
}

void drv8838::flip(String motor) {
	if(motor=="LEFT") rotate1 = !rotate1;
	if(motor=="RIGHT") rotate2 = !rotate2;
}

IR38kHz::IR38kHz() {}

IR38kHz::IR38kHz(int a,int b,int c,int d,int e) {
	sensor[0] = a;
	sensor[1] = b;
	sensor[2] = c;
	sensor[3] = d;
	sensor[4] = e;
	for(int i=0; i<5; i++)
		pinMode(sensor[0],INPUT);
}

bool IR38kHz::getStateRAW(int index) {
    if(sensor[index]>=A0 && sensor[index]<=A7) return (!(analogRead(sensor[index])>500));
	return !digitalRead(sensor[index]);
}

bool IR38kHz::getState(int index) {
    int good = 0;
	for(int i=0; i<20; i++)
		if(IR38kHz::getStateRAW(index))
			good++;
	return (good>=15);
}

int IR38kHz::horizon() {
	int index = 0;
	for(int i=0; i<5; i++)
		if(IR38kHz::getState(i))
			//index +=(int)pow(2,i);
			switch(i) {
				case 0: index +=1; break;
				case 1: index +=2; break;
				case 2: index +=4; break;
				case 3: index +=8; break;
				case 4: index +=16; break;
			}
	return index;
}

QTR1A::QTR1A() {}

QTR1A::QTR1A(int vcc_a,int gnd_a,int a,int b) {
	sensor1 = a;
	sensor2 = b;
	gnd = gnd_a;
	vcc = vcc_a;
	inv = true;
	
	pinMode(sensor1,INPUT);
	pinMode(sensor2,INPUT);
	pinMode(gnd,OUTPUT);
	pinMode(vcc,OUTPUT);
	
	digitalWrite(vcc,HIGH);
	digitalWrite(gnd,LOW);
}

bool QTR1A::getStateRAW(String side) {
	int sensor;

	if(side=="LEFT") sensor = sensor1;
	if(side=="RIGHT") sensor = sensor2;

	if(inv) return digitalRead(sensor);
	else return !digitalRead(sensor);
}

bool QTR1A::getState(String side) {
	int sensor;

	if(side=="LEFT") sensor = sensor1;
	if(side=="RIGHT") sensor = sensor2;

	int good = 0;
	for(int i=0; i<20; i++)
		if(QTR1A::getStateRAW(side))
			good++;
	return (good>=15);
}

void QTR1A::invert() {
	inv = !inv;
}

int QTR1A::horizon() {
	if(!QTR1A::getState("LEFT") && !QTR1A::getState("RIGHT")) return 0;
	if(!QTR1A::getState("LEFT") && QTR1A::getState("RIGHT")) return 2;
	if(QTR1A::getState("LEFT") && !QTR1A::getState("RIGHT")) return 1;
	if(QTR1A::getState("LEFT") && QTR1A::getState("RIGHT")) return 3;
}

Buzzer::Buzzer() {}

Buzzer::Buzzer(int pin_a) {
    pin = pin_a;
    pinMode(pin,OUTPUT);
    digitalWrite(pin,LOW);
}

void Buzzer::play() {
    digitalWrite(pin,HIGH);
}

void Buzzer::stop() {
    digitalWrite(pin,LOW);
}

StartModule::StartModule() {}

StartModule::StartModule(int start_a,drv8838 motors_a,Buzzer beep_a) {
	start = start_a;
	motors = motors_a;
	beep = beep_a;
	pinMode(start,INPUT);
}

void StartModule::waitForSignal() {
	while(!digitalRead(start)) {
		delay(1);
		motors.stop();
		beep.stop();
	}
}
