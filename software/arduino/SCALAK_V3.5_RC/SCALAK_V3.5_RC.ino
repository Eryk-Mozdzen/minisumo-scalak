#include <RCSwitch.h>
#include "SCALAK_lib_2.h"
#include "Controller_lib.h"

drv8838 motors(6,8,3,4);
IR38kHz dist(5,A4,A5,A6,A7);
QTR1A edge(A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);
Switch panel(1,11);
RF433MHz module(2,RECIEVER);

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
  module.initialize();

  motors.flip(LEFT);
  edge.set(AUTO);

  lastEnemy = panel.getState(0);
}

void loop() {
  //remote.wait();
  module.listen();

  int message = module.getMessage();
  
  if(message<100 || message>1000) message = 555;

  int powerL = 0;
  int powerR = 0;
  int power = 150;

  if(getDigit(message,1)==1) { powerL +=power; powerR -=power; }
  if(getDigit(message,2)==1) { powerL +=power; powerR +=power; }

  if(getDigit(message,0)==5) beep.stop();
  if(getDigit(message,1)==5) {}
  if(getDigit(message,2)==5) {}

  if(getDigit(message,0)==9) beep.play();
  if(getDigit(message,1)==9) { powerL -=power; powerR +=power; }
  if(getDigit(message,2)==9) { powerL -=power; powerR -=power; }

  motors.setSpeeds(powerL,powerR);

  //float error = dist.getError();

  //if(!isnan(error)) fight(error,0);
  //else              search();

  motors.print();
  //dist.print();
  //edge.print();
  //panel.print();
  //Serial.println(lastEnemy);
  //Serial.println(error);
  //Serial.println(module.getMessage());

  delay(10);
}
