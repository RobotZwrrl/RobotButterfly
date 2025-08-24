/* Robot Butterfly Dance
 * ---------------------------
 *
 * Using Processing IDE v2.2.1
 */

import msafluid.*;
import javax.media.opengl.GL2;
import mqtt.*;
import org.openkinect.*;
import org.openkinect.processing.*;

boolean PROJECTOR = false;
boolean KINECT = true;



// --------- msa fluid -----------
final float FLUID_WIDTH = 120;
float invWidth, invHeight;    // inverse of screen dimensions
float aspectRatio, aspectRatio2;
MSAFluidSolver2D fluidSolver = new MSAFluidSolver2D(40, 30);  // lighter version;
ParticleSystem particleSystem;
PImage imgFluid;
boolean drawFluid = true;
// -------------------------------

// ------------ bots -------------
final int NUM_BOTS = 4;
Bot[] bot = new Bot[NUM_BOTS];
PFont font;
// -------------------------------

// ------------ mqtt -------------
MQTTClient client;
String mqtt_username;
String mqtt_password;
String mqtt_broker_ip;
String mqtt_broker_port;
String mqtt_client_id;
String mqtt_broker_url;
int connection_attempts = 0;
boolean mqtt_connected = false;
// -------------------------------

// ------------ kinect -----------
KinectTracker tracker;
Kinect kinect;

boolean display_tracker = false;
boolean display_dot = false;

float tracker_x;
float tracker_y;
float ptracker_x;
float ptracker_y;
// -------------------------------



void setup() {
  size(1280, 720-52, P3D);
  noStroke();
  
  invWidth = 1.0f/width;
  invHeight = 1.0f/height;
  aspectRatio = width * invHeight;
  aspectRatio2 = aspectRatio * aspectRatio;

  // create fluid and set options
  fluidSolver = new MSAFluidSolver2D((int)(FLUID_WIDTH), (int)(FLUID_WIDTH * height/width));
  fluidSolver.enableRGB(true).setFadeSpeed(0.0003).setDeltaT(0.5).setVisc(0.0001);
  
  // create image to hold fluid picture
  imgFluid = createImage(fluidSolver.getWidth(), fluidSolver.getHeight(), RGB);

  // create particle system
  particleSystem = new ParticleSystem();
  
  float w = (displayWidth / NUM_BOTS);
  for(int i=0; i<bot.length; i++) {
    float x = i * w;
    bot[i] = new Bot(x, 0, w, displayHeight);
  }
  
  bot[0].topic = "robotbutterfly-003/control";
  bot[1].topic = "robotbutterfly-008/control";
  bot[2].topic = "robotbutterfly-007/control";
  bot[3].topic = "robotbutterfly-002/control";
  
  bot[0].alt_action = true;
  bot[1].alt_action = false;
  bot[2].alt_action = true;
  bot[3].alt_action = false;
  
  font = createFont("Arial", 12, true);
  textFont(font);
  textAlign(LEFT, LEFT);
  
  client = new MQTTClient(this);
  mqttConnect();
  
  if(KINECT) {
    kinect = new Kinect(this);
    tracker = new KinectTracker(); 
  }

}


void draw() {
  
  fluidSolver.update();

  if(drawFluid) {
    for(int i=0; i<fluidSolver.getNumCells(); i++) {
      int d = 2;
      imgFluid.pixels[i] = color(fluidSolver.r[i] * d, fluidSolver.g[i] * d, fluidSolver.b[i] * d);
    }  
    imgFluid.updatePixels();
    image(imgFluid, 0, 0, width, height);
  } 

  particleSystem.updateAndDraw();
  
  
  for(int i=0; i<bot.length; i++) {
  
    boolean mouse_in = false;
    if(!KINECT) {
      mouse_in = bot[i].checkMouse(mouseX, mouseY);
    } else {
      mouse_in = bot[i].checkMouse(tracker_x, tracker_y);
    }
    
    if(mouse_in) {
      
      if(!bot[i].lights_on) {
        
        if(mqtt_connected) {
          client.publish(bot[i].topic, bot[i].generateAction(true));
          bot[i].last_publish = millis();  
        }
        
      } else {
        
        if(millis()-bot[i].last_publish >= 2000 && mqtt_connected == true) {
          client.publish(bot[i].topic, bot[i].generateAction(true));
          bot[i].last_publish = millis();
        }
        
      }
      
    } else {
      
      if(bot[i].lights_on) {
        
        if(mqtt_connected) {
          client.publish(bot[i].topic, bot[i].generateAction(false));
          bot[i].last_publish = millis();
        }
        
      } else {
        
        if(millis()-bot[i].last_publish >= 2000 && mqtt_connected == true) {
          client.publish(bot[i].topic, bot[i].generateAction(false));
          bot[i].last_publish = millis();
        }
        
      }
      
    }
     
    bot[i].display();
  }
  
  if(KINECT) kinectDraw();
  
}


void mouseMoved() {
  float mouseNormX = mouseX * invWidth;
  float mouseNormY = mouseY * invHeight;
  float mouseVelX = (mouseX - pmouseX) * invWidth;
  float mouseVelY = (mouseY - pmouseY) * invHeight;

  addForce(mouseNormX, mouseNormY, mouseVelX, mouseVelY, cycleColor(mouseNormX, mouseNormY));
}


void keyPressed() {
  
  if(KINECT) {
    int t = tracker.getThreshold();
    if(key == CODED) {
      if(keyCode == UP) {
        t+=5;
        tracker.setThreshold(t);
      } 
      else if(keyCode == DOWN) {
        t-=5;
        tracker.setThreshold(t);
      }
      println("threshold: " + tracker.getThreshold());
    }
  }
  
  switch(key) {
    case 'a':
      println("bot[0] leds action");
      client.publish(bot[0].topic, bot[0].generateAction(true));
    break;
    case 'z':
      println("bot[0] leds off");
      client.publish(bot[0].topic, bot[0].generateAction(false));
    break;
    
    case 's':
      println("bot[1] leds action");
      client.publish(bot[1].topic, bot[1].generateAction(true));
    break;
    case 'x':
      println("bot[1] leds off");
      client.publish(bot[1].topic, bot[1].generateAction(false));
    break;
    
    case 'd':
      println("bot[2] leds action");
      client.publish(bot[2].topic, bot[2].generateAction(true));
    break;
    case 'c':
      println("bot[2] leds off");
      client.publish(bot[2].topic, bot[2].generateAction(false));
    break;
    
    case 'f':
      println("bot[3] leds action");
      client.publish(bot[3].topic, bot[3].generateAction(true));
    break;
    case 'v':
      println("bot[3] leds off");
      client.publish(bot[3].topic, bot[3].generateAction(false));
    break;
    
    
    case 'b': {
      for(int i=0; i<bot.length; i++) {
        bot[i].show_text = !bot[i].show_text;
      }
      if(bot[0].show_text) {
        println("show_text = true");
      } else {
        println("show_text = false");
      }
    }
    break;
    
    case 't': {
      if(KINECT) {
        int t = tracker.getThreshold();
        println("threshold: " + t + "    " +  "framerate: " + (int)frameRate + "    " + "UP increase threshold, DOWN decrease threshold");
        display_tracker = !display_tracker;
      }
    }
    break;
    
  }
  
  //println(frameRate);
  //println("displayHeight = " + displayHeight);
  
}


// add force and dye to fluid, and create particles
void addForce(float x, float y, float dx, float dy, color c) {
  float speed = dx * dx  + dy * dy * aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio

  if(speed > 0) {
    if(x<0) x = 0; 
    else if(x>1) x = 1;
    if(y<0) y = 0; 
    else if(y>1) y = 1;

    float colorMult = 5;
    float velocityMult = 30.0f;

    int index = fluidSolver.getIndexForNormalizedPosition(x, y);

    color drawColor = c;

    fluidSolver.rOld[index]  += red(drawColor) * colorMult;
    fluidSolver.gOld[index]  += green(drawColor) * colorMult;
    fluidSolver.bOld[index]  += blue(drawColor) * colorMult;

    particleSystem.addParticles(x * width, y * height, 10);
    fluidSolver.uOld[index] += dx * velocityMult;
    fluidSolver.vOld[index] += dy * velocityMult;
  }
}


color cycleColor(float x, float y) {
  colorMode(HSB, 360, 1, 1);
  float hue = ((x + y) * 180 + frameCount) % 360;
  color c = color(hue, 1, 1); 
  colorMode(RGB, 1);
  return c;
}


color generateColour(float x, float y) {
  colorMode(HSB, 360, 1, 1);
  float hue = ((x + y) * 180 + frameCount) % 360;
  color drawColor = color(hue, 1, 1);
  colorMode(RGB, 1);
  return drawColor;
}


color generateColour1D(float a) {
  colorMode(HSB, 360, 1, 1);
  float hue = (a * 180 + frameCount) % 360;
  color drawColor = color(hue, 1, 1);
  colorMode(RGB, 1);
  return drawColor;
}


void kinectDraw() {
 
  tracker.track();
  if(display_tracker) tracker.display();

  // Let's draw the "lerped" location
  PVector v2 = tracker.getLerpedPos();
  fill(100,250,50);
  noStroke();
  //ellipse(v2.x,v2.y,20,20);
  
  ptracker_x = tracker_x;
  ptracker_y = tracker_y;
  
  tracker_x = map(v2.x, 0, 640, 0, displayWidth);
  tracker_y = map(v2.y, 0, 480, 0, displayHeight);
  
  if(display_dot) {
    colorMode(RGB, 255);
    fill(100,250,50);
    ellipse(tracker_x, tracker_y, 20, 20);
  }
  
  float tracker_x_vel = (tracker_x - ptracker_x) * invWidth;
  float tracker_y_vel = (tracker_y - ptracker_y) * invHeight;
  
  addForce(tracker_x*invWidth, tracker_y*invHeight, tracker_x_vel, tracker_y_vel, cycleColor(tracker_x_vel * invWidth, tracker_y_vel * invHeight));
}


void stop() {
  if(KINECT) tracker.quit();
  super.stop();
}


