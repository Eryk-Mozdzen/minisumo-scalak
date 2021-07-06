void search() {
  int powerL = 0;
  int powerR = 0;

  switch(edge.horizon()) {
    case 0:                                     break;
    case 1: lastEnemy = true;   powerR = -150;  break;
    case 2: lastEnemy = false;  powerL = -150;  break;
    case 3:                                     break;
  }

  if(lastEnemy) powerL = 255;
  else          powerR = 255;

  pid.reset();
  motors.setSpeeds(powerL,powerR);
}
