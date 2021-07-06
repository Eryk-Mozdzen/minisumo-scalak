
void printEdge() {
  if(edge[0]) {
    stroke(red);
    fill(red);
  } else {
    stroke(green);
    fill(green);
  }
  rect(width/2-50,height/2-50,25,25);
    
  if(edge[1]) {
    stroke(red);
    fill(red);
  } else {
    stroke(green);
    fill(green);
  }
  rect(width/2+25,height/2-50,25,25);
}
