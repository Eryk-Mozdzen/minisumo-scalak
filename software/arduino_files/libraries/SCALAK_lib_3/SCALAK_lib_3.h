// SCALAK_lib.h
// Eryk Mozdzen 2019

#ifndef SCALAK_lib_3_h
#define SCALAK_lib_3_h

#include "Arduino.h"

const int LEFT = 0;
const int RIGHT = 1;

const int NORMAL = 0;
const int INVERT = 1;
const int AUTO = 2;

const int SINGLE = 0;
const int MULTI = 1;

const int ON = 0;
const int OFF = 1;

class drv8838 {
	public:
		drv8838();
		drv8838(int,int,int,int);
		void initialize();
		void setSpeeds(int,int);
		void setSpeeds(int);
		void flip(int);
		void print();
		void stop();
		static int fix(int);
	private:
		int ENABLEpin[2];
		int PHASEpin[2];
		int power[2];
		bool rotate[2];

		void setSpeed(int,int);
};

class IR38kHz {
	public:
		IR38kHz();
		IR38kHz(int,int,int,int,int);
		void initialize();
		bool getState(int);
		float getError(int);
		void print();
	private:
		int sensor[5];

		bool getStateRAW(int);
		float getErrorSingle();
		float getErrorMulti();
};

class QTR1A {
	public:
		QTR1A();
		QTR1A(int,int);
		void initialize();
		bool getState(int);
		void setMode(int);
		int horizon();
		void print();
	private:
		int sensor[2];
		bool inv;

		bool getStateRAW(int);
};

class Buzzer {
    public:
        Buzzer();
		Buzzer(int);
        void initialize();
        void play();
        void stop();
	private:
		int pin;
};

class StartModule {
	public:
		StartModule();
		StartModule(int,drv8838,Buzzer);
		void initialize();
		void wait();
		void setMode(int);
		void print();
	private:
		int startPin;
		bool active;
		bool debug;
		drv8838 motors;
		Buzzer beep;

		bool read();
};

class Switch {
	public:
		Switch();
		Switch(int,...);
		void initialize();
		bool getState(int);
		void print();
	private:
		int* pins;
		int number;
};

#endif