// Controller_lib.h
// Eryk Mozdzen 2019

#ifndef Controller_lib_h
#define Controller_lib_h

#include "Arduino.h"

class PID {
	public:
		PID();
		PID(double,double,double);
		double calculate(double,double);
		void reset();
	private:
		double Kp, Ki, Kd;
		double integral;
		double lastError;
		double deltaTime;
};

class Kalman {
	public:
		Kalman();
		Kalman(double, double, double);
		double update(double, double);
	private:
		double Q_angle, Q_gyroBias, R_measure;
		double angle, bias;
		double P[2][2];
		double lastT;
};

#endif
