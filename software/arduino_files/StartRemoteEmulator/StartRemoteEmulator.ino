#include<IRremote.h>

IRsend ir;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
}

void loop() {

  if(!digitalRead(6)) ir.sendRC5(0x2C0, 12);
  else if(!digitalRead(4)) ir.sendRC5(0x1C1, 12);
  else if(!digitalRead(5)) ir.sendRC5(0x1C0, 12);

  delay(108);
}
