#include "SCALAK_lib.h"

//drv8838 motors(9,8,11,12);
//Encoder reader(2,3);
//IR38kHz dist(4,5,6,7,10);
//QTR1A edge(A0,A1);
Buzzer beep(12);

//int power1 = 0;
//int power2 = 0;
//char side;

void setup() {
  
  //attachInterrupt(digitalPinToInterrupt(2), doEncoder, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(3), doEncoder, CHANGE);
  Serial.begin(9600);
  
  //motors.flip("LEFT");
}

void loop() {

  /*beep.play();
  delay(100);
  beep.stop();
  delay(100);*/

  //side = Serial.read();
  //if(side=='a') power1 = Serial.parseInt();
  //if(side=='b') power2 = Serial.parseInt();
  
  //motors.setSpeeds(power1,power2);
  //motors.setSpeeds(power1);
  
  //motors.setSpeeds(-50,0);
  
  //Serial.println(reader.getPos());

  //motors.setSpeeds(-255);
  //delay(3000);
  //motors.stop();
  //delay(3000);

}

//void doEncoder() {
//  reader.refresh(); 
//}
