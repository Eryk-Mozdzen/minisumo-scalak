#include "NeuralNetwork_lib.h"
#include <MemoryFree.h>

int pattern[] = {7,40,15,12,8,5,2};
NeuralNetwork net(pattern,sizeof(pattern)/sizeof(int),"tanh");
Matrix netIn(pattern[0],1);
Matrix netOut;

float dist[] = {0,1,1,0,0};
float edge[] = {0,1};

void setup() {

  Serial.begin(9600);
  //randomSeed(analogRead(0));
  //net = NeuralNetwork(pattern,sizeof(pattern)/sizeof(int),"tanh");

}

void loop() {

  Serial.print("Free Memory: "); Serial.print(freeMemory()); Serial.println("B");

  for(byte i=0; i<5; i++) netIn.elements[i  ].elements[0] = dist[i];//.getState(i);
  for(byte i=0; i<2; i++) netIn.elements[i+5].elements[0] = edge[i];//.getState(i);

  netOut = net.calculate(netIn);
 
  netOut.multiply(255);
  
  netOut.print();

  //motors.setSpeeds(netOut.elements[0].elements[0],netOut.elements[1].elements[0]);

  delay(500);

}
