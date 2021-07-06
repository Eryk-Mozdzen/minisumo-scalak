// SCALAK_lib.cpp
// Eryk Mozdzen 2019

#include "SCALAK_lib_3.h"

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

void drv8838::initialize() {
	for(int i=0; i<2; i++) {
		pinMode(ENABLEpin[i], OUTPUT);
		pinMode(PHASEpin[i], OUTPUT);
	}
	drv8838::flip(LEFT);
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

static int drv8838::fix(int input) {
	if(input>255) input = 255;
	if(input<-255) input = -255;
	return input;
}

void drv8838::setSpeed(int index, int speed) {
	speed = drv8838::fix(speed);
	power[index] = speed;
	if(speed>0) {
		analogWrite(ENABLEpin[index],speed);
		digitalWrite(PHASEpin[index],rotate[index]);
	}
	if(speed<0) {
		analogWrite(ENABLEpin[index],-speed);
		digitalWrite(PHASEpin[index],!rotate[index]);
	}
	if(speed==0) 
		analogWrite(ENABLEpin[index],0);
}

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

QTR1A::QTR1A() {}

QTR1A::QTR1A(int a,int b) {
	sensor[0] = a;
	sensor[1] = b;
	inv = true;
}

void QTR1A::initialize() {
	for(int i=0; i<2; i++)
		pinMode(sensor[i],INPUT);
}

bool QTR1A::getStateRAW(int index) {
	if(inv) return digitalRead(sensor[index]);
	else return !digitalRead(sensor[index]);
}

bool QTR1A::getState(int index) {
	int good = 0;
	for(int i=0; i<20; i++)
		if(QTR1A::getStateRAW(index))
			good++;
	return (good>=15);
}

void QTR1A::setMode(int mode) {
	if(mode==0) inv = true;
	if(mode==1) inv = false;
	if(mode==2) {
		bool left = QTR1A::getState(LEFT);
		bool right = QTR1A::getState(RIGHT);
		if(left==right) inv = !left;
	}
}

int QTR1A::horizon() {
	if(!QTR1A::getState(LEFT) && !QTR1A::getState(RIGHT)) return 0;
	if(!QTR1A::getState(LEFT) && QTR1A::getState(RIGHT)) return 2;
	if(QTR1A::getState(LEFT) && !QTR1A::getState(RIGHT)) return 1;
	if(QTR1A::getState(LEFT) && QTR1A::getState(RIGHT)) return 3;
}

void QTR1A::print() {
	Serial.print(QTR1A::getState(LEFT));
	Serial.print(QTR1A::getState(RIGHT));
	Serial.print('\n');
}

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

StartModule::StartModule() {}

StartModule::StartModule(int startPin_a,drv8838 motors_a,Buzzer beep_a) {
	startPin = startPin_a;
	motors = motors_a;
	beep = beep_a;
	active = true;
	debug = false;
}

void StartModule::initialize() {
	pinMode(startPin,INPUT_PULLUP);
}

void StartModule::wait() {
	while(StartModule::read() && active) {
		motors.stop();
		beep.stop();
		if(debug)
			Serial.println("Detected: Stopped");
		delay(10);
	}
	if(debug) {
		if(active) Serial.println("Detected: Running");
		else Serial.println("Not Detected");
	}
}

void StartModule::setMode(int mode) {
	if(mode==0) active = true;
	if(mode==1) active = false;
	if(mode==2) active = !digitalRead(startPin);
}

void StartModule::print() {
	debug = true;
}

bool StartModule::read() {
	return !digitalRead(startPin);
}

Switch::Switch() {}

Switch::Switch(int number_a, ...) {
	number = number_a;
	pins = new int[number];
	
	va_list arguments;
  	va_start(arguments, number);

	for(int i=0; i<number; i++)
		pins[i] = va_arg(arguments, int);
}

void Switch::initialize() {
	for(int i=0; i<number; i++)
		pinMode(pins[i],INPUT_PULLUP);
}

bool Switch::getState(int index) {
	if(pins[index]>=A0 && pins[index]<=A7) return (!(analogRead(pins[index])>500));
	return !digitalRead(pins[index]);
}

void Switch::print() {
	for(int i=0; i<number; i++)
		Serial.print(Switch::getState(i));
	Serial.print('\n');
}