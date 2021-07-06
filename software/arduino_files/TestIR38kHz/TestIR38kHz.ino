
void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);

  digitalWrite(A2,HIGH);
  digitalWrite(A1,LOW);
  //pinMode(A1,INPUT);
}

void loop() {
  Serial.println(analogRead(A0));
  //Serial.print("\t");
  //Serial.println(analogRead(A1));
  //Serial.println(digitalRead(A0));
}
