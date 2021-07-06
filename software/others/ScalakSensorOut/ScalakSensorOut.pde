import processing.serial.*;

Serial arduinoPort;
String massageFromArduino;
String lastGoodMassage = "0000000;0;0\n";
boolean[] dist;
boolean[] edge;
int[] motors;
color red = color(255,0,0);
color green = color(0,255,0);

void setup() {
  
  size(800,800);
  background(0);
  arduinoPort = new Serial(this,Serial.list()[0],9600);
  
  dist = new boolean[5];
  edge = new boolean[2];
  motors = new int[2];
  
  for(int i=0; i<5; i++) dist[i] = false;
  for(int i=0; i<2; i++) edge[i] = false;
  for(int i=0; i<2; i++) motors[i] = 0;
  
}

void draw() {
  
  arduinoRead();
  clear();
  
  stroke(255);
  noFill();
  rect(width/2-50,height/2-50,100,100);
  
  printEdge();
  printDist();
  
}
