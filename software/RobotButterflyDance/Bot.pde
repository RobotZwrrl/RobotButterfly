class Bot {

  float x, y, w, h;
  boolean mouse_inside = false;
  long last_emit = 0;
  int colour_cycle = 0;
  String topic;
  long last_send = 0;
  boolean show_text = true;
  boolean particle_dir = false;
  boolean alt_action = false;
  long last_publish = 0;
  boolean lights_on = false;
  
  Bot(float tempX, float tempY, float tempW, float tempH) {
    x = tempX;
    y = tempY;
    w = tempW;
    h = tempH;
  }

  void display() {
    
    // highlight when mouse is inside
    if(mouse_inside) {
      fill(255, 255, 255);
      //rect(x, h-70, w, 70);
      rect(x, h-220, w, 10); // idk
    }

    if(mouse_inside) {
      if(millis()-last_emit >= 500) {
        emitParticles();
      }
      if(millis()-last_send >= 2000) {
        // TODO: send mqtt
      }
    }
    
    if(show_text) displayTopicText();

  }

  boolean checkMouse() {
    if(mouseX >= x && mouseX < x + w &&
       mouseY >= y && mouseY < y + h) {
      mouse_inside = true;     
    } else {
      mouse_inside = false; 
    }
    return mouse_inside;
  }
  
  void emitParticles() {
    particle_dir = !particle_dir;
    float vel = 5.0;
    color c = generateColour1D(colour_cycle);
    for(int i=0; i<12; i++) {
      colour_cycle+=100;
      if(particle_dir) addForce( (x+100)*invWidth, (400-(i*10))*invHeight, -20*invWidth, -5*invHeight, c);
      if(!particle_dir) addForce( (x+100)*invWidth, (400-(i*10))*invHeight, 20*invWidth, -5*invHeight, c); 
    }
    last_emit = millis();
  }
  
  void displayTopicText() {
    //hint(DISABLE_DEPTH_TEST);
    textFont(font); 
    fill(255, 255, 255);
    text(topic, x, h-250);
    //hint(ENABLE_DEPTH_TEST);
  }
  
  
  String generateAction(boolean on) {
    
    if(!on) {
      lights_on = false;
      return "0";
    }
    
    alt_action = !alt_action;
    lights_on = true;
    
    if(alt_action) {
      return "1";
    } else {
      return "2";
    }
    
  }
  
}


