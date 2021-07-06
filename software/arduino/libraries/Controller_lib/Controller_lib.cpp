// Controller_lib.cpp
// Eryk Mozdzen 2019

#include "Controller_lib.h"

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

Kalman::Kalman() {}

Kalman::Kalman(double Q_angle_a, double Q_gyroBias_a, double R_measure_a) {
    Q_angle = Q_angle_a;
    Q_gyroBias = Q_gyroBias_a;
    R_measure = R_measure_a;

    angle = 0;
    bias = 0;

    P[0][0] = 0;
    P[0][1] = 0;
    P[1][0] = 0;
    P[1][1] = 0;

    lastT = (double)micros();
}

double Kalman::update(double newAngle, double newRate) {
    double dt = (double)(micros()-lastT)/1000000;

    double rate = newRate - bias;
    angle += dt * rate;

    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_gyroBias * dt;
    
    double y = newAngle - angle;
    double S = P[0][0] + R_measure;
	double K[] = { P[0][0]/S, P[1][0]/S };

	angle += K[0] * y;
    bias += K[1] * y;

    double P00 = P[0][0];
    double P01 = P[0][1];

    P[0][0] -= K[0] * P00;
    P[0][1] -= K[0] * P01;
    P[1][0] -= K[1] * P00;
    P[1][1] -= K[1] * P01;

    lastT = (double)micros();

    return angle;
};