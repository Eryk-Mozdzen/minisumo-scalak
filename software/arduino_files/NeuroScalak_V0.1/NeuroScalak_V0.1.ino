#include "NeuralNetwork_lib.h"
#include "SCALAK_lib.h"
#include <MemoryFree.h>

drv8838 motors(6,8,3,4);
IR38kHz dist(2,5,A5,A6,A7);
QTR1A edge(A4,7,A3,A2);
Buzzer beep(12);
StartModule remote(9,motors,beep);

int pattern[] = {7,5,2};
NeuralNetwork net(pattern,sizeof(pattern)/sizeof(int),"tanh");
Matrix netIn(pattern[0],1);
Matrix netOut;

void setup() {

  Serial.begin(9600);
  motors.flip("LEFT");

}

void loop() {

  //remote.waitForSignal();

  Serial.print("Free Memory: "); Serial.print(freeMemory()); Serial.println("B");

  for(byte i=0; i<5; i++) netIn.elements[i  ].elements[0] = dist.getState(i);
  for(byte i=0; i<2; i++) netIn.elements[i+5].elements[0] = edge.getState(i);

  netOut = net.calculate(netIn);
  netOut.multiply(255);
  
  netOut.print();

  motors.setSpeeds(netOut.elements[0].elements[0],netOut.elements[1].elements[0]);

  delay(10);

}
