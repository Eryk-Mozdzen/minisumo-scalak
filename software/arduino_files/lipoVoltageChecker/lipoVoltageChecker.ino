class Battery {
  public:
    Battery() {}
    Battery(int pin_a) {
      pin = pin_a;
    }
    void initialize() {
      pinMode(pin, INPUT);
    }
    float getVoltage() {
      return (analogRead(A0)/1023.0)*12.4;
    }
    void print() {
      float v = Battery::getVoltage();
      if(v!=0)Serial.println(v, 5);
      else Serial.println("Not connected");
    }
  private:
    int pin;
};

Battery lipo(A0);

void setup() {
  Serial.begin(9600);
  lipo.initialize();

}

void loop() {
  
  lipo.print();

  delay(10);
}
