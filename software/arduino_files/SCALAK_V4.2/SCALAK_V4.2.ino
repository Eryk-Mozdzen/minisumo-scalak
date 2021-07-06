#include "SCALAK_lib_4.h"
#include "Controller_lib.h"

#define edgeSpacing 0.073
#define wheelRadius 0.0155
#define wheelSpacing 0.075

drv8838 motors(6,7,9,10);
Encoders encode(2,5,3,4);
IR38kHz dist(A3,A4,A5,A6,A7);
QTR1A edge(A2,A1);
Buzzer beep(8);
StartModule remote(A0);
Switch panel(1,12);

PID pid(127.5,0,0);

bool lastEnemy;
int searchCounter = 0;

void refresh0() { encode.refreshCounter0(); }
void refresh1() { encode.refreshCounter1(); }

void setup() {
  Serial.begin(9600);

  motors.initialize();
  encode.initialize(refresh0, refresh1);
  dist.initialize();
  edge.initialize();
  beep.initialize();
  remote.initialize();
  panel.initialize();

  edge.setMode(AUTO);
  remote.setMode(AUTO);

  lastEnemy = panel.getState(0);

  //remote.print();
}

void loop() {

  remote.wait([]() {
    motors.stop();
    beep.stop();
    
    static byte blip = 0;
    if((blip++)%10==0) digitalWrite(13, !digitalRead(13));
  });

  float processVarible = dist.getError(SINGLE);

  if(!isnan(processVarible)) {
    fight(processVarible,255);
    searchCounter = 0;
  } else {
    if(searchCounter<100) {
      search();
      searchCounter++;
    } else explore();
  }

  //motors.print();
  //encode.print();
  //dist.print();
  //edge.print();
  //panel.print();
  //Serial.println(lastEnemy);
  //Serial.println(error);

  delay(25);
}
