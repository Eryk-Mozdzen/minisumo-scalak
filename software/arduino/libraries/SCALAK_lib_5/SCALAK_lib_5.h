// SCALAK_lib.h
// Eryk Mozdzen 2019

#ifndef SCALAK_lib_5_h
#define SCALAK_lib_5_h

#include "Arduino.h"

const int LEFT = 0;
const int RIGHT = 1;

const int FORWARD = 0;
const int BACKWARD = 1;

const int NORMAL = 0;
const int INVERT = 1;
const int AUTO = 2;

const int SINGLE = 0;
const int MULTI = 1;

const int ON = 0;
const int OFF = 1;

const int MANUAL = 20;
const int MODULE = 21;

class drv8838 {
	public:
		drv8838();
		drv8838(int,int,int,int);
		void initialize(int,int);
		void setSpeeds(int,int);
		void setSpeeds(int);
		void print();
		void stop();
		void stop(int);
		int getSpeed(int);
		static int fix(int);
	private:
		int ENABLEpin[2];
		int PHASEpin[2];
		int power[2];
		bool rotate[2];

		void setSpeed(int, int);
		void flip(int);
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
		void initialize(int);
		bool getState(int);
		int getHorizon();
		void print();
	private:
		int sensor[2];
		bool inv;

		bool getStateRAW(int);
		void setMode(int);
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
		StartModule(int);
		void initialize(int);
		void wait(void(*)(), void(*)());
		void print();
		int getMode();
	private:
		int startPin;
		int mode;
		bool debug;
		bool click, clicked;

		bool read();
		void setMode(int);
};

class Switch {
	public:
		Switch();
		Switch(int);
		void initialize();
		bool getState();
		void print();
	private:
		int pin;
};

class Led {
	public:
		Led();
		Led(int);
		void initialize();
		void blink(float);
		void setState(int);
	private:
		int pin;
		unsigned long lastT;
};

#endif