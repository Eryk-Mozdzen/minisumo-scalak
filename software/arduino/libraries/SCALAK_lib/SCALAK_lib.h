// SCALAK_lib.h
// Eryk Mozdzen 2019

#ifndef SCALAK_lib_h
#define SCALAK_lib_h

#include "Arduino.h"

class drv8838 {
	public:
		int ENABLEpin1, PHASEpin1;
		int ENABLEpin2, PHASEpin2;
		bool rotate1, rotate2;
		int speedA,speedB;

		drv8838();
		drv8838(int,int,int,int);
		void setSpeeds(int,int);
		void setSpeeds(int);
		void stop();
		void flip(String);
};

class IR38kHz {
	public:
		int sensor[5];

		IR38kHz();
		IR38kHz(int,int,int,int,int);
		bool getStateRAW(int);
		bool getState(int);
		int horizon();
};

class QTR1A {
	public:
		int sensor1,sensor2;
		int vcc, gnd;
		bool inv;

		QTR1A();
		QTR1A(int,int,int,int);
		bool getStateRAW(String);
		bool getState(String);
		void invert();
		int horizon();
};

class Buzzer {
    public:
        int pin;

        Buzzer();
        Buzzer(int);
        void play();
        void stop();
};

class StartModule {
	public:
		int start;
		drv8838 motors;
		Buzzer beep;
		
		StartModule();
		StartModule(int,drv8838,Buzzer);
		void waitForSignal();
};

#endif
