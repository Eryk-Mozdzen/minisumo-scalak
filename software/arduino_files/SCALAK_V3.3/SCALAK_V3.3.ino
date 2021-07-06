#include "SCALAK_lib_2.h"
#include "PID_lib.h"

drv8838 motors;
IR38kHz dist;
QTR1A edge;
Buzzer beep;
StartModule remote;
Switch panel;

Controller pid(127.5,0,0);

bool lastEnemy = false;

void setup() {
  Serial.begin(9600);
  
  motors = drv8838(10,8,3,4);
  dist = IR38kHz(2,5,A5,A6,A7);
  edge = QTR1A(A3,A2);
  beep = Buzzer(12);
  remote = StartModule(9,motors,beep);
  panel = Switch(1,11);

  motors.flip(LEFT);
  edge.set(AUTO);

  lastEnemy = panel.getState(0);
}

void loop() {
  //remote.waitForSignal();

  float error = dist.getError();

  if(!isnan(error)) fight(error,255);
  else              search();

  //motors.print();
  //dist.print();
  //edge.print();
  //panel.print();

  delay(10);
}
