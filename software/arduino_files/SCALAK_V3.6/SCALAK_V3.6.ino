#include <RCSwitch.h>
#include "SCALAK_lib_2.h"
#include "Controller_lib.h"

drv8838 motors(6,8,3,4);
IR38kHz dist(5,A4,A5,A6,A7);
QTR1A edge(A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);
Switch panel(1,11);

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

  edge.set(AUTO);
  remote.set(AUTO);

  lastEnemy = panel.getState(0);

  lurk();
}

void loop() {
  remote.wait();

  float error = dist.getError(MULTI);

  if(!isnan(error)) fight(error,255);
  else              search();

  //motors.print();
  //dist.print();
  //edge.print();
  //panel.print();
  //Serial.println(lastEnemy);
  //Serial.println(error);

  delay(10);
}
