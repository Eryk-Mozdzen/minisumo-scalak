void search() {

  if(edge.horizon()==0) motors.setSpeeds(150);
  else {
    encode.reset();

    bool sensor = (edge.getState(LEFT));
    
    while(edge.horizon()!=3) {}
    int steps = (encode.getPosition(0) + encode.getPosition(1))/2;
    motors.stop();
    delay(50);
    
    double d = (2*PI*wheelRadius)*(steps/306.0);
    double phi = PI - atan2(d, edgeSpacing);

    motors.setSpeeds(-255);
    delay(150);

    rotate(phi, sensor);
    delay(150);
  }
  
  pid.reset();
}

void rotate(double phi, bool dir) {
  double S = phi*(wheelSpacing/2);
  double l = 2*PI*wheelRadius;
  double rounds = S/l;
  int rotateSteps = rounds*306;
  
  if(dir) motors.setSpeeds(255, -255);
  else motors.setSpeeds(-255, 255);

  encode.reset();
  
  while(abs(encode.getPosition(0))<rotateSteps) delay(1);
  
  motors.stop();
}
