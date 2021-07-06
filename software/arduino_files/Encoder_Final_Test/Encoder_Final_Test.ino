#define fullCycle 306

int counter = 0;
unsigned long lastTime;
int lastPos;

void setup() {
  Serial.begin(9600);
  
  pinMode(2, INPUT);
  pinMode(7, INPUT);
  
  attachInterrupt(0, refresh, CHANGE);
  
  lastPos = counter;
  lastTime = millis();
}

void loop() {

  double deltaP = double(counter-lastPos)/fullCycle;
  double deltaT = double(micros()-lastTime)/1000000.0;

  double rps = deltaP/deltaT;

  Serial.println(rps);

  lastPos = counter;
  lastTime = (double)micros();

  delay(100);
}

void refresh() {
  //if(digitalRead(2)==digitalRead(7)) counter++;
  //if(digitalRead(2)!=digitalRead(7)) counter--;

  counter +=((digitalRead(2)==digitalRead(7))*2 - 1);
} 
