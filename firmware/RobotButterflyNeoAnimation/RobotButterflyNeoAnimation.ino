/*
 * Robot Butterfly Neopixel Animation
 * --------------------------------
 * Blinky animations with the colour palette
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 17 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <Adafruit_NeoPixel.h>



// ----------- neopixel colours -----------
// go with 400 kHz as 800 kHz would leave pixel colour ghosts
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

enum pastelIndex {
  NEO_RED,
  NEO_ORANGE,
  NEO_GOLDEN_YELLOW,
  NEO_CANARY_YELLOW,
  NEO_GREEN,
  NEO_SKY_BLUE,
  NEO_BLUE,
  NEO_CYAN,
  NEO_PURPLE,
  NEO_LAVENDER,
  NEO_PINK,
  NEO_MAGENTA,
  NEO_WARM_WHITE,
  NEO_WHITE,
  NEO_OFF
};

String pastelNames[NUM_PALETTE_COLOURS] = {
  "Red",
  "Orange",
  "Golden Yellow",
  "Canary Yellow",
  "Green",
  "Sky Blue",
  "Blue",
  "Cyan",
  "Purple",
  "Lavender",
  "Pink",
  "Magenta",
  "Warm White",
  "White",
  "Off"
};

uint16_t pastelColours[NUM_PALETTE_COLOURS][3] = {
    {  719,  170,  250},   // red
    { 3561,  160,  250},   // orange
    { 5701,  160,  250},   // golden yellow
    { 9622,  170,  250},   // canary yellow
    {16545,  170,  250},   // green
    {32845,   60,  250},   // sky blue
    {43691,  120,  250},   // blue
    {27306,  120,  250},   // cyan
    {51613,  130,  250},   // purple
    {55613,   60,  250},   // lavendar
    {  659,   90,  250},   // pink
    {63535,  140,  250},   // magenta
    { 5461,  120,  250},   // warm white
    { 8192,   90,  250},   // white
    {    0,    0,    0},   // off
};

uint32_t colourPalette[NUM_PALETTE_COLOURS];

bool AUTO_ADVANCE = true;
long last_colour_change = 0;
uint8_t colour_index = 0;
uint8_t colour_index_prev = 0;
// -----------------------------------

// ----------- neopixel animation -----------
enum neoAnimName {
  NEO_ANIM_NONE,
  NEO_ANIM_POLKADOT,
  NEO_ANIM_SQUIGGLE
};

enum neoAnimType {
  NEO_ANIM_ALERT,
  NEO_ANIM_HOME
};

void runNeoAnim_none(struct NeoAnimation *animation);
void runNeoAnim_polkadot(struct NeoAnimation *animation);
void runNeoAnim_squiggle(struct NeoAnimation *animation);
typedef void (*AnimationFunction)(NeoAnimation*); // function pointer type that accepts a NeoAnimation pointer

struct NeoAnimation {
  uint8_t id;
  bool active;
  uint8_t type;

  uint8_t colour_primary;    // enum of the colour
  uint8_t colour_secondary;  // enum of the colour

  uint8_t num_frames;
  uint16_t frame_delay;
  uint8_t frame_index;
  long last_frame;
  
  int num_repeats;
  uint16_t repeat_count;
  uint16_t repeat_delay;
  long last_repeat;

  long duration;
  long start_time;

  bool dir;
  int helper1;
  int helper2;
  int helper3;

  AnimationFunction function;  // function pointer
};

NeoAnimation neo_animation_home;
NeoAnimation neo_animation_alert;
// -----------------------------------



void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initNeopixels();
  initNeoAnimations();

  Serial << "Ready" << endl;
}


void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateNeoAnimation();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '0':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
        startNeoAnim(&neo_animation_alert);
      break;
      case '1':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_MAGENTA, NEO_CYAN);
        startNeoAnim(&neo_animation_alert);
      break;
      case '2':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_SQUIGGLE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_MAGENTA, NEO_CYAN);
        startNeoAnim(&neo_animation_alert);
      break;
      case '3':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_SQUIGGLE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_PURPLE);
        setNeoAnimDuration(&neo_animation_alert, 300);
        startNeoAnim(&neo_animation_alert);
      break;
      case '4':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_SQUIGGLE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_PINK, NEO_CANARY_YELLOW);
        setNeoAnimRepeats(&neo_animation_alert, 3);
        startNeoAnim(&neo_animation_alert);
      break;
      case '5':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_SQUIGGLE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_WHITE, NEO_LAVENDER);
        setNeoAnimSpeed(&neo_animation_alert, 1000);
        startNeoAnim(&neo_animation_alert);
      break;
      case 's':
        stopNeoAnim(&neo_animation_alert);
      break;
      case 'h':
        Serial << "1-9: animations" << endl;
        Serial << "s: stop" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}


// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the alert animation is done
void callback_NeoAnimAlertDone(struct NeoAnimation *a, uint8_t type) {
  //setNeoAnimColours(NEO_GOLDEN_YELLOW, NEO_SKY_BLUE);
}

// the home animation is done
void callback_NeoAnimHomeDone(struct NeoAnimation *a, uint8_t type) {
  //setNeoAnimColours(NEO_OFF, NEO_OFF);
}

// ----------------------------------


// ----------------------------------
// ------------ setters -------------
// ----------------------------------

// params: neo animation, name of animation, type
void setNeoAnim(struct NeoAnimation *a, uint8_t n, uint8_t t) {
  
  // init
  // TODO - all of the neo animations
  switch(n) {
    case NEO_ANIM_NONE:
      initNeoAnim_none(a);
    break;
    case NEO_ANIM_POLKADOT:
      initNeoAnim_polkadot(a);
    break;
    case NEO_ANIM_SQUIGGLE:
      initNeoAnim_squiggle(a);
    break;
  }

  a->type = t;
}

// params: neo animation, primary colour, secondary colour
void setNeoAnimColours(struct NeoAnimation *a, uint8_t c1, uint8_t c2) {
  a->colour_primary = c1;
  a->colour_secondary = c2;
}

// params: neo animation, duration of animation
void setNeoAnimDuration(struct NeoAnimation *a, long duration) {
  a->duration = duration;
}

// params: neo animation, num repeats of animation
void setNeoAnimRepeats(struct NeoAnimation *a, int r) {
  a->num_repeats = r;
}

// params: neo animation, frame delay (larger = slower)
void setNeoAnimSpeed(struct NeoAnimation *a, uint16_t del) {
  a->frame_delay = del;
}

// params: neo animation
void startNeoAnim(struct NeoAnimation *a) {
  a->active = true;
}

// params: neo animation
void stopNeoAnim(struct NeoAnimation *a) {
  a->active = false;
}
// ----------------------------------


void updateNeoAnimation() {
  
  // check if there's an animation happening currently
  // if not, do the home animation
  if(neo_animation_alert.active) {

    if(neo_animation_alert.function) {
      neo_animation_alert.function(&neo_animation_alert);
    }

  } else {

    if(neo_animation_home.function) {
      neo_animation_home.function(&neo_animation_home);
    }

  }
  
}


bool neoAnimationChecks(struct NeoAnimation *a) {

  if(!a) return false; // safety check

  // check that the animation delay is done. if not, wait again
  if(millis()-a->last_repeat < a->repeat_delay) {
    if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "animation delay not done" << endl;
    return false;
  }

  // check that the animation duration is complete
  if(millis()-a->start_time >= a->duration  
     && a->start_time != -1
     && a->duration >= 0)
  {
    a->start_time = -1;
    a->repeat_count = 0;
    a->active = false;
    if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "animation done (time elapsed)" << endl;
    // callback anim done (time)
    if(a->type == NEO_ANIM_ALERT) {
      callback_NeoAnimAlertDone(a, 1);   // 1 designates 'time elapsed' done type
    } else if(a->type == NEO_ANIM_HOME) {
      callback_NeoAnimHomeDone(a, 1);    // 1 designates 'time elapsed' done type
    }
    return false;
  }

  // check that the number of repeats is complete
  if(a->repeat_count >= a->num_repeats 
     && a->num_repeats != -99) {
    a->start_time = -1;
    a->repeat_count = 0;
    a->active = false;
    if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "animation done (num repeats)" << endl;
    // callback anim done (repeats)
    if(a->type == NEO_ANIM_ALERT) {
      callback_NeoAnimAlertDone(a, 2);   // 2 designates 'num repeats' done type
    } else if(a->type == NEO_ANIM_HOME) {
      callback_NeoAnimHomeDone(a, 2);    // 2 designates 'num repeats' done type
    }
    return false;
  }

  // increment the frame
  if(millis()-a->last_frame >= a->frame_delay) {
    a->frame_index++;
    if(a->frame_index > a->num_frames-1) {
      a->frame_index = 0;
      a->repeat_count++;
      a->last_repeat = millis();
    }
  } else {
    return false;
  }

  // if it's here then the animation is active
  // first, reset the vars
  if(a->start_time == -1) { //   if(a->active == false) {
    a->repeat_count = 0;
    a->dir = true;
    a->helper1 = 0;
    a->helper2 = 0;
    a->helper3 = 0;
    a->start_time = millis();
  }
  a->active = true;
  if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "Neo animation: " << a->id;
  if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << " frame: " << a->frame_index+1 << "/" << a->num_frames << endl;

  return true;

}


void runNeoAnim_none(struct NeoAnimation *animation) {

  if(!neoAnimationChecks(animation)) return;

  // only 1 frame in this animation
  pixels.clear();
  for(uint8_t i=0; i<NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, colourPalette[NEO_OFF]);
  }
  pixels.show();
  animation->last_frame = millis();

}


void runNeoAnim_squiggle(struct NeoAnimation *animation) {

  if(!neoAnimationChecks(animation)) return;

  // do this each frame
  if(animation->dir) {
    animation->helper1++;
  } else {
    animation->helper1--;
  }
  if(animation->helper1 < (0+1)) {
    animation->helper1 = (0+1);
    animation->dir = !animation->dir;
  }
  if(animation->helper1 > (NEOPIXEL_COUNT-1-1)) {
    animation->helper1 = (NEOPIXEL_COUNT-1-1);
    animation->dir = !animation->dir;
  }

  int a = animation->helper1-1;
  int b = animation->helper1;
  int c = animation->helper1+1;

  // this happens every frame
  pixels.clear();
  for(uint8_t i=0; i<NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, colourPalette[NEO_OFF]);
  }
  pixels.setPixelColor(a, colourPalette[animation->colour_primary]);
  pixels.setPixelColor(b, colourPalette[animation->colour_secondary]);
  pixels.setPixelColor(c, colourPalette[animation->colour_primary]);
  pixels.show();
  animation->last_frame = millis();
  // ---

}


void runNeoAnim_polkadot(struct NeoAnimation *animation) {

  if(!neoAnimationChecks(animation)) return;

  switch(animation->frame_index) {
    case 0: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, colourPalette[animation->colour_primary]);
        } else {
          pixels.setPixelColor(i, colourPalette[animation->colour_secondary]);
        }
      }
      pixels.show();
      animation->last_frame = millis();
    }
    break;
    case 1: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, colourPalette[animation->colour_secondary]);
        } else {
          pixels.setPixelColor(i, colourPalette[animation->colour_primary]);
        }
      }
      pixels.show();
      animation->last_frame = millis();
    }
    break;
  }

}


// this is a template for copying and pasting
void runNeoAnim_template(struct NeoAnimation *animation) {

  if(!neoAnimationChecks(animation)) return;

  switch(animation->frame_index) {
    case 0: {
      pixels.clear();
      pixels.setPixelColor(0, animation->colour_primary);
      pixels.show();
      animation->last_frame = millis();
    }
    break;
    case 1: {
      pixels.clear();
      pixels.setPixelColor(0, animation->colour_secondary);
      pixels.show();
      animation->last_frame = millis();
    }
    break;
  }

}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

