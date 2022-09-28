#ifndef PID_H_
#define PID_H_

#include <avr/io.h>

#define PID_NUMBER	1

struct pid_data {
	double Kp, Ki, Kd;
	double lastError;
	double integral;
	double deltaTime;
} pid_database[PID_NUMBER];

void pid_initialize(uint8_t, double, double, double);
double pid_calculate(uint8_t, double, double);
void pid_reset(uint8_t);

#endif