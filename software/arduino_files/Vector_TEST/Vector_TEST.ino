#include "Vector_lib.h"

Vector<int> xd;

void setup() {
  Serial.begin(9600);

}

void loop() {
  xd.push_back(true);
  
  Serial.println(xd.size());

}
