#include "NeuralNetwork_lib.h"
#include "SCALAK_lib_3.h"
#include <MemoryFree.h>

drv8838 motors(6,8,3,2);
IR38kHz dist(A3,A4,A5,A6,A7);
QTR1A edge(A2,A1);
Buzzer beep(12);
StartModule remote(9,motors,beep);
Switch panel(1,10);

NeuralNetwork net(4,8,9,5,2);
Matrix netI(8,1);
Matrix netO(2,1);

void setup() {
  Serial.begin(9600);

  motors.initialize();
  dist.initialize();
  edge.initialize();
  beep.initialize();
  remote.initialize();
  panel.initialize();

  edge.setMode(AUTO);
  remote.setMode(AUTO);

  net.randomize(analogRead(0));
}

void loop() {
  remote.wait();

  //Serial.print("Free Memory: "); Serial.print(freeMemory()); Serial.println("B");

  for(byte i=0; i<5; i++) netI.elements[i  ].elements[0] = dist.getState(i);
  for(byte i=0; i<2; i++) netI.elements[i+5].elements[0] = edge.getState(i);
  for(byte i=0; i<1; i++) netI.elements[i+7].elements[0] = panel.getState(i);

  netO = net.calculate(netI);

  //netO.print();
  Serial.print(netO.elements[0].elements[0]);
  Serial.print('\t');
  Serial.println(netO.elements[1].elements[0]);

  //motors.setSpeeds(netO.elements[0].elements[0],netO.elements[1].elements[0]);

  delay(25);
}
