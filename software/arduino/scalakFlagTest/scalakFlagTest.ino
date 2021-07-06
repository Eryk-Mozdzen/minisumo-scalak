#include <Servo.h>
#include "SCALAK_lib_3.h"

drv8838 motors(6,8,3,2);
IR38kHz dist(A3,A4,A5,A6,A7);
QTR1A edge(A2,A1);
Buzzer beep(12);
SG90 flag(4,5);
StartModule remote(9,motors,beep,flag);
Switch panel(1,10);

void setup() {
  Serial.begin(9600);

  motors.initialize();
  dist.initialize();
  edge.initialize();
  beep.initialize();
  remote.initialize();
  panel.initialize();
  flag.initialize();

  edge.set(AUTO);
  remote.set(AUTO);
  flag.set(UP);
}

void loop() {

  flag.set(UP,DOWN);

  flag.print();

  delay(10);
}
