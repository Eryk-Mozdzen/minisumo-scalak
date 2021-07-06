void search() {

  if(edge.horizon()==0) motors.setSpeeds(255);
  else {
    motors.stop();
    delay(250);

    bool sensor = (edge.getState(LEFT));
    
    unsigned long start = millis();
    motors.setSpeeds(255);
    while(edge.horizon()!=3) {}
    double t = (double)(millis() - start)/1000.0;
    motors.stop();

    double d = (2*PI*wheelRadius)*(motorSpeed*t);
    double phi = PI - atan2(d, edgeSpacing);

    Serial.println(phi*180/PI);

    motors.setSpeeds(-255);
    delay(250);

    rotate(phi, sensor);
  }
  
  pid.reset();
}

void rotate(double phi, bool dir) {
  double S = phi*(wheelSpacing/2) *2;
  double l = 2*PI*wheelRadius;
  double rounds = S/l;
  double rotateTime = rounds/motorSpeed;
  
  if(dir) motors.setSpeeds(255, -255);
  else motors.setSpeeds(-255, 255);
  
  delay(rotateTime*1000);
  
  motors.stop();
  delay(100);
}
