#include <RCSwitch.h>
#include "Controller_lib.h"

RF433MHz module(8,TRANSMITER);

void setup() {
  
  Serial.begin(9600);

  module.initialize();

  module.set(10,5);
}

void loop() {

  String input = Serial.readStringUntil('\n');

  int data = 555;

  for(int i=0; i<input.length(); i++) {
    if(input[i]=='p') data +=400;
    if(input[i]=='w') data +=40;
    if(input[i]=='s') data -=40;
    if(input[i]=='d') data +=4;
    if(input[i]=='a') data -=4;
  }

  if(input.length()>0) {
    module.send(data);
    //Serial.println(data);
  }
    

  delay(10);
}
