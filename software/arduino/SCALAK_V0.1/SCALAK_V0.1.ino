#include "SCALAK_lib.h"

drv8838 motors(6,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A4,7,A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

bool lastEnemy = true;

void setup() {

  Serial.begin(9600);

  motors.flip("LEFT");
  //edge.invert();

}

void loop() {

  //remote.waitForSignal();

  /*if(dist.getState(2)) motors.setSpeeds(255,255);
  else if(dist.getState(0) || dist.getState(1)) {
    motors.setSpeeds(-255,255);
    lastEnemy = true;
  } else if(dist.getState(3) || dist.getState(4)) {
    motors.setSpeeds(255,-255);
    lastEnemy = false;
  } else if(lastEnemy) motors.setSpeeds(255,-255);
  else motors.setSpeeds(-255,255);*/

  //if(dist.getState(0) || dist.getState(1) || dist.getState(2) || dist.getState(3) || dist.getState(4)) beep.play();
  //else beep.stop();

  //Serial.println(edge.getState("LEFT"));
  //Serial.println("working");

  Serial.print(edge.getState("LEFT"));
  Serial.print(" ");
  Serial.println(edge.getState("RIGHT"));
  
  //beep.play();

  /*Serial.print(dist.getState(0));
  Serial.print(" ");
  Serial.print(dist.getState(1));
  Serial.print(" ");
  Serial.print(dist.getState(2));
  Serial.print(" ");
  Serial.print(dist.getState(3));
  Serial.print(" ");
  Serial.print(dist.getState(4));
  Serial.println(" ");*/
}
