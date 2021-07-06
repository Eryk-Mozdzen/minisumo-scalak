boolean checkInput(String massage) {
  int index = 8;
  if(massage==null) return false;
  if(massage.length()<11) return false;
  if(massage.charAt(7)!=';') return false;
  for(int i=0; i<7; i++) if(massage.charAt(i)!='0' && massage.charAt(i)!='1') return false;
  while(massageFromArduino.charAt(index)!=';') {
    if((massageFromArduino.charAt(index)<'0' || massageFromArduino.charAt(index)>'9') && massageFromArduino.charAt(index)!='-') return false;
    index++;
  }
  index++;
  while(massageFromArduino.charAt(index)!='\n') {
    if((massageFromArduino.charAt(index)<'0' || massageFromArduino.charAt(index)>'9') && massageFromArduino.charAt(index)!='-') return false;
    index++;
  }
  return true;
}

void arduinoRead() {

  if(arduinoPort.available()>0) massageFromArduino = arduinoPort.readStringUntil('\n');
  //massageFromArduino = "1010101;-34;56\n";
  println(massageFromArduino);
  if(!checkInput(massageFromArduino)) massageFromArduino = lastGoodMassage;
  
  for(int i=0; i<5; i++) dist[i] = readMassage(massageFromArduino.charAt(i));  
  for(int i=0; i<2; i++) edge[i] = readMassage(massageFromArduino.charAt(i+5));
  
  int index = 8;
  String motorA = "";
  String motorB = "";
  
  while(massageFromArduino.charAt(index)!=';') {
    motorA +=massageFromArduino.charAt(index);
    index++;
  }
  index++;
  while(massageFromArduino.charAt(index)!='\n') {
    motorB +=massageFromArduino.charAt(index);
    index++;
  }
  
  println("motor1 = "+motorA);
  println("motor2 = "+motorB);
  
  motors[0] = Integer.parseInt(motorA);
  motors[1] = Integer.parseInt(motorB);
  
  println("motor[0] = "+motors[0]);
  println("motor[1] = "+motors[1]);
  
  lastGoodMassage = massageFromArduino;
}

boolean readMassage(char sign) {
  if(sign=='1') return true;
  if(sign=='0') return false;
  return false;
}
