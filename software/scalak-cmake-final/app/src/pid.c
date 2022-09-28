#include "pid.h"

void pid_initialize(uint8_t index, double Kp_a, double Ki_a, double Kd_a) {
	pid_database[index].Kp = Kp_a;
	pid_database[index].Ki = Ki_a;
	pid_database[index].Kd = Kd_a;
	
	pid_database[index].deltaTime = 1;
	pid_database[index].integral = 0;
	pid_database[index].lastError = 0;
}

double pid_calculate(uint8_t index, double processVarible, double setPoint) {
	double error = processVarible - setPoint;
	double derivative = (error-pid_database[index].lastError)/pid_database[index].deltaTime;
	pid_database[index].integral +=(error*pid_database[index].deltaTime);

	double controlVariable = pid_database[index].Kp*error + pid_database[index].Ki*pid_database[index].integral + pid_database[index].Kd*derivative;

	pid_database[index].lastError = error;

	return controlVariable;
}

void pid_reset(uint8_t index) {
	pid_database[index].integral = 0;
	pid_database[index].lastError = 0;
}