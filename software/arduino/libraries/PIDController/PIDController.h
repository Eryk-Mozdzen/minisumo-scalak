// Controller_lib.h
// Eryk Mozdzen 2019

#ifndef PIDController_h
#define PIDController_h

#include "Arduino.h"

class PID {
	public:
		PID();
		PID(float,float,float);
		float calculate(float,float);
		void reset();
	private:
		float Kp, Ki, Kd;
		float integral;
		float lastError;
		float deltaTime;
};

#endif
