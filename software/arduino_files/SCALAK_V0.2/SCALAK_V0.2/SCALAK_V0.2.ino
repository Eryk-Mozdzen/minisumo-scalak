#include "SCALAK_lib.h"
#include <math.h>

#define speedFactor 164.0508
#define wheelRadius 0.0165
#define wheelSpacing 0.0755
#define edgeSensorSpacing 0.07

drv8838 motors(/*6*/10,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A4,7,A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

bool lastEnemy = true;
int scan = 150;
int scanCounter = scan;

void setup() {

  Serial.begin(9600);

  motors.flip("LEFT");
  edge.invert();

}

void loop() {

  remote.waitForSignal();
  
  if(edge.horizon()==0 || dist.getState(2)) fight();
  else poorReturnToCenter(150);

  //fight();
  //search(100);
  //motors.setSpeeds(50);
  //if(edge.horizon()!=0) poorReturnToCenter(50);
  //if(edge.horizon()!=0) returnToCenter(50);
  
  //sensorView();
  
  delay(10);

}

void fight() {
  switch(dist.horizon()) {
    case 0:   search2(150);              break;
    
    case 1:   turn(150,0,"LEFT");       lastEnemy = true; break;
    case 3:   turn(150,0.05,"LEFT");    lastEnemy = true; break;
    case 2:   turn(150,0.05,"LEFT");    lastEnemy = true; break;
    case 7:   turn(150,0.1,"LEFT");     lastEnemy = true; break;
    
    case 6:   turn(150,0.1,"LEFT");     lastEnemy = true; break;
    case 12:  turn(150,0.1,"RIGHT");    lastEnemy = false; break;    
    case 4:   attack(255);              break;
    case 14:  attack(255);              break;
    
    case 8:   turn(150,0.05,"RIGHT");   lastEnemy = false; break;
    case 24:  turn(150,0.05,"RIGHT");   lastEnemy = false; break;
    case 16:  turn(150,0,"RIGHT");      lastEnemy = false; break;
    case 28:  turn(150,0.1,"RIGHT");    lastEnemy = false; break;
  }
  if(dist.horizon()!=0) scanCounter = scan;
}

void attack(int power) {
  motors.setSpeeds(power);
}

void search(int power) {
  if(scanCounter==0) {
    scanCounter = scan;
    while(true) {
      bool ended = false;
      while(true) {
        motors.setSpeeds(power);
        Serial.println("urrraaaa");
        if(edge.horizon()!=0) break;
        if(dist.horizon()!=0) {
          ended = true;
          break;
        }
      }
      if(ended) break;
      Serial.println("turning");
      turnAngle(power,180,"LEFT");
    }
  } else {
    Serial.print(scanCounter);
    if(lastEnemy) {
      Serial.println("lewo");
      turn(power,0,"LEFT");
    } else {
      Serial.println("prawo");
      turn(power,0,"RIGHT");
    }
    scanCounter--;
  }
}

void search2(int power) {
  if(lastEnemy) turn(power,0,"LEFT");
  else turn(power,0,"RIGHT");
}

void turn(int power,float R,String side) {
  float diff = (R+0.5*wheelSpacing)/(R-0.5*wheelSpacing);
  float speed1;
  float speed2;

  if(side=="LEFT") {
    speed1 = power;
    speed2 = speed1/diff;
  } else if(side=="RIGHT") {
    speed2 = power;
    speed1 = speed2/diff;
  }

  motors.setSpeeds((int)speed1,(int)speed2);
}

void sensorView() {
  for(int i=0; i<5; i++) {
    Serial.print(dist.getState(i));
    Serial.print(" ");
  }
  Serial.print("= ");
  Serial.print(dist.horizon());

  Serial.print('\t');
  Serial.print('\t');
  Serial.print(edge.getState("LEFT"));
  Serial.print(" ");
  Serial.print(edge.getState("RIGHT"));
  Serial.print(" = ");
  Serial.print(edge.horizon());
  Serial.print('\t');
  Serial.print('\t');
  Serial.print("lastEnemy = ");
  Serial.println(lastEnemy);
}

void returnToCenter(int power) {
  unsigned long long time0;
  unsigned long long timeEnd;
  unsigned long long deltaTimeRAW;
  double deltaTime;
  double alpha;
  double turnTime;
  String second;

  if(edge.getState("LEFT")) second = "RIGHT";
  if(edge.getState("RIGHT")) second = "LEFT";
  
  motors.setSpeeds(power);
  time0 = millis();

  while(!edge.getState(second)) {}

  timeEnd = millis();
  motors.stop();
  deltaTimeRAW = timeEnd - time0;
  deltaTime = (double)deltaTimeRAW/1000.0;

  alpha = atan((edgeSensorSpacing*speedFactor)/(power*deltaTime));

  turnAngle(power,PI-alpha,second);
  //motors.setSpeeds(power);
  //delay(1000);
  motors.stop();
 
}

void poorReturnToCenter(int power) {
  String first;
  String second;
  int good = 0;

  Serial.println("start");

  if(edge.getState("LEFT")) { first = "LEFT"; second = "RIGHT"; }
  if(edge.getState("RIGHT")) { first = "RIGHT"; second = "LEFT"; }

  turn(power,0.05,first);

  while(good!=20) {
    good = 0;
    for(int i=0; i<20; i++)
      if(edge.getState(second))
        good++;
  }

  motors.stop();
  turnAngle(power,PI,first);
}

void turnAngle(int power,double angle,String side) {
  double turnTime = angle*wheelSpacing*speedFactor/power;
  turn(power,0,side);
  delay((int)(turnTime*1000));
  motors.stop();
}
