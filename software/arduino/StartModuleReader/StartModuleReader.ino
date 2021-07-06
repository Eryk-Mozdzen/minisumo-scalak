void setup() {
  
  Serial.begin(9600);
  pinMode(8,INPUT_PULLUP);
  pinMode(7,INPUT);

}

void loop() {

  bool start = digitalRead(8);
  bool kill = digitalRead(7);

  Serial.print("Start: ");
  Serial.print(start);
  Serial.print("\tKill: ");
  Serial.println(kill);

}
