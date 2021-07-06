#include "SCALAK_lib.h"

drv8838 motors(6,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
//QTR1A edge(,);
Buzzer beep(12);

bool lastEnemy = true;
int attackSpeed = 25;
int aimSpeed1 = 75;
int aimSpeed2 = 50;
int confusedSpeed = 100;

void setup() {

  Serial.begin(9600);

  motors.flip("LEFT");

}

void loop() {

  if(dist.getState(2)) motors.setSpeeds(attackSpeed);
  else {
    if(dist.getState(0) || dist.getState(1)) {
      if(dist.getState(0)) motors.setSpeeds(-aimSpeed1,aimSpeed1);
      if(dist.getState(1)) motors.setSpeeds(-aimSpeed2,aimSpeed2);
      lastEnemy = true;
    } 
    if(dist.getState(3) || dist.getState(4)) {
      if(dist.getState(3)) motors.setSpeeds(aimSpeed2,-aimSpeed2);
      if(dist.getState(4)) motors.setSpeeds(aimSpeed1,-aimSpeed1);
      lastEnemy = false;
    } else (lastEnemy) ? motors.setSpeeds(confusedSpeed,-confusedSpeed) : motors.setSpeeds(-confusedSpeed,confusedSpeed);
  } 
  
  delay(10);

}
