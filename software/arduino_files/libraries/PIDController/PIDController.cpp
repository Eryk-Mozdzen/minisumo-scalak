// Controller_lib.cpp
// Eryk Mozdzen 2019

#include "PIDController.h"

PID::PID() {}

PID::PID(float Kp_a, float Ki_a, float Kd_a) {
	Kp = Kp_a;
	Ki = Ki_a;
	Kd = Kd_a;
	deltaTime = 1;

	integral = 0;
	lastError = 0;
}

float PID::calculate(float processVarible, float setPoint) {
	float error = processVarible - setPoint;
	float derivative = (error-lastError)/deltaTime;
	integral +=(error*deltaTime);

	float controlVariable = Kp*error + Ki*integral + Kd*derivative;

	lastError = error;

	return controlVariable;
}

void PID::reset() {
	integral = 0;
    lastError = 0;
}