/* Robot Butterfly Dance
 * ---------------------------
 *
 * Using Processing IDE v2.2.1
 */

import msafluid.*;
import javax.media.opengl.GL2;
import mqtt.*;

MQTTClient client;

final float FLUID_WIDTH = 120;

float invWidth, invHeight;    // inverse of screen dimensions
float aspectRatio, aspectRatio2;

MSAFluidSolver2D fluidSolver = new MSAFluidSolver2D(40, 30);  // lighter version;

ParticleSystem particleSystem;

PImage imgFluid;

boolean drawFluid = true;

final int NUM_BOTS = 4;
Bot[] bot = new Bot[NUM_BOTS];

PFont font;

// ------------ mqtt -------------
String mqtt_username;
String mqtt_password;
String mqtt_broker_ip;
String mqtt_broker_port;
String mqtt_client_id;
String mqtt_broker_url;
int connection_attempts = 0;
boolean mqtt_connected = false;
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
    
    if(bot[i].checkMouse()) {
      if(!bot[i].lights_on) {
        client.publish(bot[i].topic, bot[i].generateAction(true));
        bot[i].last_publish = millis();
      } else {
        if(millis()-bot[i].last_publish >= 2000) {
          client.publish(bot[i].topic, bot[i].generateAction(true));
          bot[i].last_publish = millis();
        }
      }
    } else {
      if(bot[i].lights_on) {
        client.publish(bot[i].topic, bot[i].generateAction(false));
        bot[i].last_publish = millis();
      } else {
        if(millis()-bot[i].last_publish >= 2000) {
          client.publish(bot[i].topic, bot[i].generateAction(false));
          bot[i].last_publish = millis();
        }
      }
    }
    
    bot[i].display();
  }
  
}


void mouseMoved() {
  float mouseNormX = mouseX * invWidth;
  float mouseNormY = mouseY * invHeight;
  float mouseVelX = (mouseX - pmouseX) * invWidth;
  float mouseVelY = (mouseY - pmouseY) * invHeight;

  addForce(mouseNormX, mouseNormY, mouseVelX, mouseVelY, cycleColor(mouseNormX, mouseNormY));
}


void mousePressed() {
    //drawFluid ^= true;
}

void keyPressed() {
  switch(key) {
    case 'r': { 
      renderUsingVA ^= true; 
      println("renderUsingVA: " + renderUsingVA);
    }
    break;
    
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
    
    
    case 't': {
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


