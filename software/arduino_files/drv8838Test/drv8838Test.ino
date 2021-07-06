
String input;
int power = 100;

void setup() {

  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {

  input = Serial.readStringUntil('\n');
  Serial.println(input);
  
  if(input!="") power = input.toInt();
  
  digitalWrite(6,HIGH);
  analogWrite(8,power);
  
  delay(10);
}
