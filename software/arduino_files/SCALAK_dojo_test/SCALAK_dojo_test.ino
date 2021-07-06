#include "SCALAK_lib_2.h"
#include "PID_lib.h"

drv8838 motors(/*6*/10,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

Controller pid(127.5,0,0);

void setup() {
  
  Serial.begin(9600);

  motors.flip("LEFT");
  motors.multiply(1,1);
  edge.set("AUTO");

}

void loop() {

  //remote.waitForSignal();

  if(edge.horizon()!=0) {
    motors.setSpeeds(-75);
    delay(500);
    motors.setSpeeds(-75,75);
    delay(210);
    motors.stop();
    delay(50);
  } else motors.setSpeeds(75);

  delay(10);

}
