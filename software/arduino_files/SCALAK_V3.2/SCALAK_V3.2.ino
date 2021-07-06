#include "SCALAK_lib_2.h"
#include "PID_lib.h"

drv8838 motors(10,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

Controller pid(127.5,0,0);

bool lastEnemy = false;

void setup() {
  Serial.begin(9600);

  //motors.multiply(1,1);
  motors.flip(LEFT);
  edge.set(AUTO);
}

void loop() {
  //remote.waitForSignal();

  float error = dist.getError();

  if(!isnan(error)) fight(error,255);
  else              search();

  motors.print();
  //dist.print();
  //edge.print();

  delay(10);
}
