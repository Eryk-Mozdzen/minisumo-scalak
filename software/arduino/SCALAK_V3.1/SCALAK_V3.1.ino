#include "SCALAK_lib_2.h"
#include "PID_lib.h"

drv8838 motors(/*6*/10,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

Controller pid(127.5,0,0);

bool lastEnemy = false;
int targetPower = 255;

void setup() {
  
  Serial.begin(9600);

  motors.flip("LEFT");
  motors.multiply(1,1);
  edge.set("AUTO");

}

void loop() {

  //remote.waitForSignal();

  /*if(!isnan(dist.getError())) fight();
  else {
    switch(edge.horizon()) {
      case 0: fight(); break;
      case 1: break;
      case 2: break;
      case 3: break;
    }
  }*/
  

  //motors.print();
  //dist.print();
  //edge.print();

  delay(10);

}

void fight() {

  int powerL = 0;
  int powerR = 0;
  
  float error = dist.getError();

  if(!isnan(error)) {
    lastEnemy = (error>0);

    float turn = pid.calculate(error,0);

    powerL = drv8838::fix((float)(targetPower)+turn);
    powerR = drv8838::fix((float)(targetPower)-turn);
    
  } else {
    if(lastEnemy) powerL = 255;
    else powerR = 255;

    pid.reset();
  }
  //Serial.println(error);

  motors.setSpeeds(powerL,powerR);

}
