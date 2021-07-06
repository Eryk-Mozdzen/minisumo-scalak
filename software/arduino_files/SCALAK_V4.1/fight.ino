void fight(float error, int targetPower) {
  lastEnemy = (error>0);

  float turn = pid.calculate(error,0);

  int powerL = drv8838::fix((float)(targetPower)+turn);
  int powerR = drv8838::fix((float)(targetPower)-turn);

  motors.setSpeeds(powerL,powerR);
}
