#include "PID_Controller.h"

PID::PID() {}

PID::PID(double Kp_a, double Ki_a, double Kd_a) {
	Kp = Kp_a;
	Ki = Ki_a;
	Kd = Kd_a;
	deltaTime = 1;

	integral = 0;
	lastError = 0;
}

double PID::calculate(double processVarible, double setPoint) {
	double error = processVarible - setPoint;
	double derivative = (error-lastError)/deltaTime;
	integral +=(error*deltaTime);

	double controlVariable = Kp*error + Ki*integral + Kd*derivative;

	lastError = error;

	return controlVariable;
}

void PID::reset() {
	integral = 0;
	lastError = 0;
}