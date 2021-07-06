String input;
bool start = false;

void setup() {
  Serial.begin(9600);

  pinMode(12,OUTPUT);

  digitalWrite(12,LOW);
}

void loop() {
  input = Serial.readStringUntil('\n');

  if(input=="s") start = !start;
  
  digitalWrite(12,start);

}
