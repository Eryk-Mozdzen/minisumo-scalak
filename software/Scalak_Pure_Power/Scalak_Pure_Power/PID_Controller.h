#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

class PID {
	public:
		PID();
		PID(double, double, double);
		double calculate(double, double);
		void reset();
	private:
		double Kp, Ki, Kd;
		double integral;
		double lastError;
		double deltaTime;
};

#endif