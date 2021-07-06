#include "SCALAK_lib_2.h"

drv8838 motors(/*6*/10,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A4,7,A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

bool lastEnemy = false;

int targetPower = 255;
double Kp = 127.5;
double Ki = 0;
double Kd = 0;

double previousError = 0;
double integral = 0;

void setup() {
  
  Serial.begin(9600);

  motors.flip("LEFT");
  //edge.invert();

}

void loop() {

  //remote.waitForSignal();

  fight();

  delay(50);

}

void fight() {

  int powerL = 0;
  int powerR = 0;
  
  float error = dist.getError();

  if(!isnan(error)) {
    lastEnemy = (error>0);

    double derivative = error - previousError;
    integral +=error;
    
    double change = Kp*error + Ki*integral + Kd*derivative;
  
    powerL = drv8838::fix((double)(targetPower)+change);
    powerR = drv8838::fix((double)(targetPower)-change);
    
    previousError = error;

  } else {
    if(lastEnemy) powerL = 255;
    else powerR = 255;

    previousError = 0;
    integral = 0;
  }
  //Serial.println(error);
  //Serial.println(derivative);
  //Serial.println(integral);

  motors.setSpeeds(powerL,powerR);
  motors.print();
  //dist.print();

}
