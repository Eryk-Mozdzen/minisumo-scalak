#include "SCALAK_lib_3.h"
#include "Controller_lib.h"

drv8838 motors(6,8,3,2);
IR38kHz dist(A3,A4,A5,A6,A7);
QTR1A edge(A2,A1);
Buzzer beep(12);
StartModule remote(9,motors,beep);
Switch panel(1,10);

PID pid(127.5,0,0);

bool lastEnemy;

void setup() {
  Serial.begin(9600);

  motors.initialize();
  dist.initialize();
  edge.initialize();
  beep.initialize();
  remote.initialize();
  panel.initialize();

  edge.setMode(AUTO);
  remote.setMode(AUTO);

  lastEnemy = panel.getState(0);
  
  remote.print();
}

void loop() {
  remote.wait();

  float error = dist.getError(SINGLE);

  if(!isnan(error)) fight(error,255);
  else              search();

  //motors.print();
  //dist.print();
  //edge.print();
  //panel.print();
  //Serial.println(lastEnemy);
  //Serial.println(error);

  delay(25);
}
