void fight(float processVarible, int targetPower) {
  lastEnemy = (processVarible>0);

  float turn = pid.calculate(processVarible, 0);

  int powerL = drv8838::fix((float)(targetPower)+turn);
  int powerR = drv8838::fix((float)(targetPower)-turn);

  motors.setSpeeds(powerL,powerR);
}
