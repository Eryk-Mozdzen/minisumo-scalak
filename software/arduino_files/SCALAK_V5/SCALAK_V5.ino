#include "SCALAK_lib_5.h"
#include "Controller_lib.h"

drv8838 motors(9,10,6,7);
IR38kHz dist(A3,A4,A5,A6,A7);
QTR1A edge(A2,A1);
Buzzer beep(8);
StartModule remote(A0);
Switch panel(12);
Led light(13);

PID pid(127.5,0,0);

bool lastEnemy;

void setup() {
  Serial.begin(9600);

  motors.initialize(BACKWARD,FORWARD);
  dist.initialize();
  edge.initialize(AUTO);
  remote.initialize(AUTO);
  panel.initialize();
  light.initialize();
  beep.initialize();

  //remote.print();
}

void loop() {

  /*remote.wait([]() {
    motors.stop();
    beep.stop();
    light.blink(5);
  }, []() {
    light.setState(OFF);
    lastEnemy = panel.getState();

    if(remote.getMode()==MANUAL) delay(5000);

    motors.setSpeeds(255);
    delay(150);
  });*/

  float processVarible = dist.getError(SINGLE);

  if(!isnan(processVarible)) fight(processVarible,255);
  else search();

  //motors.print();
  //dist.print();
  //edge.print();
  //panel.print();

  Serial.print(analogRead(A1));
  Serial.print('\t');
  Serial.println(analogRead(A2));

  delay(50);
}
