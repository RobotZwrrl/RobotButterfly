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
#define NUM_NEO_ANIMATIONS 3

enum neoAnimationIndex {
  NEO_ANIM_NONE,
  NEO_ANIM_POLKADOT,
  NEO_ANIM_SQUIGGLE
};

void runNeoAnim_none(struct NeoAnimation *animation);
void runNeoAnim_polkadot(struct NeoAnimation *animation);
void runNeoAnim_squiggle(struct NeoAnimation *animation);
typedef void (*AnimationFunction)(NeoAnimation*); // function pointer type that accepts a NeoAnimation pointer

struct NeoAnimation {
  uint8_t id;
  uint32_t colour_primary;
  uint32_t colour_secondary;
  bool active;

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

NeoAnimation neo_animations[NUM_NEO_ANIMATIONS];

uint8_t NEO_ANIMATION_ALERT = NEO_ANIM_NONE;
uint8_t NEO_ANIMATION_HOME = NEO_ANIM_NONE;
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
        setNeoAnimationAlert(NEO_ANIM_NONE);
      break;
      case '1':
        setNeoAnimationAlert(NEO_ANIM_POLKADOT);
      break;
      case '2':
        setNeoAnimationAlert(NEO_ANIM_SQUIGGLE);
      break;
      case 's':
        NEO_ANIMATION_ALERT = NEO_ANIM_NONE;
        NEO_ANIMATION_HOME = NEO_ANIM_NONE;
      break;
      case 'h':
        Serial << "1-9: animations" << endl;
        Serial << "s: stop" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}


void setNeoAnimationAlert(uint8_t a) {
  NEO_ANIMATION_ALERT = a;
  neo_animations[NEO_ANIMATION_ALERT].active = true;
}


// TODO
// void setNeoAnimationHome(uint8_t a) {

// }

// TODO
// void setNeoAnimationHome(uint8_t a, colour1, colour2, speed) {
  
// }


void updateNeoAnimation() {
  
  // check if there's an animation happening currently
  // if not, do the home animation
  if(neo_animations[NEO_ANIMATION_ALERT].active) {

    if(neo_animations[NEO_ANIMATION_ALERT].function) {
      neo_animations[NEO_ANIMATION_ALERT].function(&neo_animations[NEO_ANIMATION_ALERT]);
    }

  } else {

    if(neo_animations[NEO_ANIMATION_HOME].function) {
      neo_animations[NEO_ANIMATION_HOME].function(&neo_animations[NEO_ANIMATION_HOME]);
    }

  }
  
}



bool neoAnimationChecks(struct NeoAnimation *a) {

  if(!a) return false; // safety check

  // check that the animation delay is done. if not, wait again
  if(millis()-a->last_repeat < a->repeat_delay) {
    if(DEBUG_NEO_ANIMATION) Serial << "animation delay not done" << endl;
    return false;
  }

  // check that the animation duration is complete
  if(millis()-a->start_time >= a->duration  
     && a->start_time != -1) // && a->num_repeats != -99
  {
    a->start_time = -1;
    a->active = false;
    if(DEBUG_NEO_ANIMATION) Serial << "animation done (time elapsed)" << endl;
    // TODO: callback anim done (time)
    return false;
  }

  // increment the frame
  if(millis()-a->last_frame >= a->frame_delay) {
    a->frame_index++;
    if(a->frame_index >= a->num_frames) {
      a->frame_index = 0;
      a->repeat_count++;
      a->last_repeat = millis();
    }
  } else {
    return false;
  }

  // check that the number of repeats is within bounds
  if(a->repeat_count-1 >= a->num_repeats 
     && a->num_repeats != -99) {
    a->repeat_count = 0;
    a->active = false;
    if(DEBUG_NEO_ANIMATION) Serial << "animation done (num repeats)" << endl;
    // TODO: callback anim done (repeats)
    return false;
  }

  // if it's here then the animation is active
  // first, reset the vars
  if(a->active == false) {
    a->repeat_count = 0;
    a->dir = true;
    a->helper1 = 0;
    a->helper2 = 0;
    a->helper3 = 0;
    a->start_time = millis();
  }
  a->active = true;
  if(DEBUG_NEO_ANIMATION) Serial << "Neo animation: " << a->id;
  if(DEBUG_NEO_ANIMATION) Serial << " frame: " << a->frame_index+1 << "/" << a->num_frames << endl;

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

  switch(animation->frame_index) {
    case 0: {
      pixels.clear();
      for(uint8_t i=0; i<NEOPIXEL_COUNT; i++) {
        pixels.setPixelColor(i, colourPalette[NEO_OFF]);
      }
      pixels.setPixelColor(a, animation->colour_primary);
      pixels.setPixelColor(b, animation->colour_secondary);
      pixels.setPixelColor(c, animation->colour_primary);
      pixels.show();
      animation->last_frame = millis();
    }
    break;
  }

}


void runNeoAnim_polkadot(struct NeoAnimation *animation) {

  if(!neoAnimationChecks(animation)) return;

  switch(animation->frame_index) {
    case 0: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, animation->colour_primary);
        } else {
          pixels.setPixelColor(i, animation->colour_secondary);
        }
      }
      pixels.show();
      animation->last_frame = millis();
    }
    break;
    case 1: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, animation->colour_secondary);
        } else {
          pixels.setPixelColor(i, animation->colour_primary);
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

