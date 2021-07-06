#include <Servo.h>

Servo servo;

String input;
int angle = 0;

void setup() {

  Serial.begin(9600);
  servo.attach(8);
  servo.write(0);

}

void loop() {

  input = Serial.readStringUntil('\n');
  if(input!="")
    angle = input.toInt();

  servo.write(angle);
  delay(10);

}
