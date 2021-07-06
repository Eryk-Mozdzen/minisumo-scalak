void explore() {

  if(edge.horizon()==0) motors.setSpeeds(50);
  else {
    encode.reset();

    bool side = edge.getState(LEFT);
    
    while(edge.horizon()!=3) {}

    int steps = (encode.getPosition(0) + encode.getPosition(1))/2;
    motors.stop();
    delay(50);
    
    double d = (2*PI*wheelRadius)*(steps/306.0);
    double phi = PI - atan2(d, edgeSpacing);

    motors.setSpeeds(-150);
    delay(150);

    rotate(phi, side);
    delay(150);
  }
  
  pid.reset();
}

void rotate(double phi, bool dir) {
  double S = phi*(wheelSpacing/2);
  double l = 2*PI*wheelRadius;
  double rounds = S/l;
  int rotateSteps = rounds*306;
  
  if(dir) motors.setSpeeds(150, -150);
  else motors.setSpeeds(-150, 150);

  encode.reset();

  while(motors.getSpeed(LEFT)!=0 || motors.getSpeed(RIGHT)!=0) {
    if(abs(encode.getPosition(LEFT))>=rotateSteps) motors.stop(LEFT);
    if(abs(encode.getPosition(RIGHT))>=rotateSteps) motors.stop(RIGHT);
    delay(1);
  }
}
