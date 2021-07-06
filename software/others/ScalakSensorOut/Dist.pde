void printDist() {
  if(dist[0]) stroke(red);
  else stroke(green);
  line(width/2-50,height/2,width/2-150,height/2);
  
  if(dist[1]) stroke(red);
  else stroke(green);
  line(width/2-50,height/2-50,width/2-125,height/2-125);
  
  if(dist[2]) stroke(red);
  else stroke(green);
  line(width/2,height/2-50,width/2,height/2-150);
  
  if(dist[3]) stroke(red);
  else stroke(green);
  line(width/2+50,height/2-50,width/2+125,height/2-125);
  
  if(dist[4]) stroke(red);
  else stroke(green);
  line(width/2+50,height/2,width/2+150,height/2);
}
