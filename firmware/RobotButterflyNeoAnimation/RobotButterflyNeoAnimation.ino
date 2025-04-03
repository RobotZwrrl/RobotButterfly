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


// ----------- neoanim callbacks -----------
void callback_NeoAnimDone(struct NeoAnimation *a);
void callback_NeoAnimLoop(struct NeoAnimation *a);
// -----------------------------------


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

// these are the colour settings when the 
// brightness is set to 20, which is visible
// during daylight outdoors
uint16_t pastelColours[NUM_PALETTE_COLOURS][3] = {
    {  100,  180,  250},   // red
    { 3500,  190,  250},   // orange
    { 6622,  190,  250},   // golden yellow
    { 9000,  190,  250},   // canary yellow
    {15545,  170,  250},   // green
    {32845,   50,  250},   // sky blue
    {43691,  120,  250},   // blue
    {27306,  120,  250},   // cyan
    {51613,  130,  250},   // purple
    {56613,   40,  250},   // lavendar
    {  359,   90,  250},   // pink
    {63535,  140,  250},   // magenta
    { 6561,  140,  250},   // warm white
    { 8192,   90,  250},   // white
    {    0,    0,    0},   // off
};

uint32_t colourPalette[NUM_PALETTE_COLOURS];
// -----------------------------------

// ------- neopixel animation --------
enum neoAnimName {
  NEO_ANIM_NONE,
  NEO_ANIM_POLKADOT,
  NEO_ANIM_SQUIGGLE,
  NEO_ANIM_RANGE,
  NEO_ANIM_FUNKY,
  NEO_ANIM_ZWOOP,
  NEO_ANIM_SPRINKLE,
  NEO_ANIM_RAINBOW,
  NEO_ANIM_RAINBOW_ALL,
  NEO_ANIM_BOUNCE,
  NEO_ANIM_BREATHE,
  NEO_ANIM_CYCLE8,
  NEO_ANIM_CYCLE3
};

enum neoAnimType {
  NEO_ANIM_ALERT,
  NEO_ANIM_HOME
};

void runNeoAnim_none(struct NeoAnimation *animation);
void runNeoAnim_polkadot(struct NeoAnimation *animation);
void runNeoAnim_squiggle(struct NeoAnimation *animation);
void runNeoAnim_range(struct NeoAnimation *animation);
void runNeoAnim_funky(struct NeoAnimation *animation);
void runNeoAnim_zwoop(struct NeoAnimation *animation);
void runNeoAnim_sprinkle(struct NeoAnimation *animation);
void runNeoAnim_rainbow(struct NeoAnimation *animation);
void runNeoAnim_rainbow_all(struct NeoAnimation *animation);
void runNeoAnim_bounce(struct NeoAnimation *animation);
void runNeoAnim_breathe(struct NeoAnimation *animation);
void runNeoAnim_cycle8(struct NeoAnimation *animation);
void runNeoAnim_cycle3(struct NeoAnimation *animation);
typedef void (*AnimationFunction)(NeoAnimation*); // function pointer type that accepts a NeoAnimation pointer

struct NeoAnimation {
  uint8_t id;
  bool active;
  uint8_t type;

  uint8_t colour_primary;    // enum of the colour
  uint8_t colour_secondary;  // enum of the colour

  uint8_t num_frames;
  int frame_delay;
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

// ----------- variables -------------
long last_print = 0;
// -----------------------------------


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initNeopixels();
  initNeoAnimations();

  Serial << "Ready" << endl;
}


void loop() {
  
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
      case '6':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_RANGE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
        //setNeoAnimDuration(&neo_animation_alert, 300);
        neo_animation_alert.dir = true;
        neo_animation_alert.helper1 = 5;
        startNeoAnim(&neo_animation_alert);
      break;
      case '7':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_RANGE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
        //setNeoAnimDuration(&neo_animation_alert, 300);
        neo_animation_alert.dir = false;
        neo_animation_alert.helper1 = 5;
        startNeoAnim(&neo_animation_alert);
      break;
      case '8':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_FUNKY, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_LAVENDER, NEO_SKY_BLUE);
        setNeoAnimSpeed(&neo_animation_alert, 300);
        startNeoAnim(&neo_animation_alert);
      break;
      case '9':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_ZWOOP, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_LAVENDER, NEO_OFF);
        setNeoAnimSpeed(&neo_animation_alert, 500);
        startNeoAnim(&neo_animation_alert);
      break;
      case 'q':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_MAGENTA, NEO_CYAN);
        setNeoAnimSpeed(&neo_animation_alert, 500);
        startNeoAnim(&neo_animation_alert);
      break;
      case 'w':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_RAINBOW, NEO_ANIM_ALERT);
        neo_animation_alert.helper2 = 150;  // hue steps
        startNeoAnim(&neo_animation_alert);
      break;
      case 'e':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_RAINBOW_ALL, NEO_ANIM_ALERT);
        neo_animation_alert.helper2 = 250;   // hue steps
        startNeoAnim(&neo_animation_alert);
      break;
      case 'r':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_BOUNCE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_CANARY_YELLOW, NEO_OFF);
        startNeoAnim(&neo_animation_alert);
      break;
      case 't':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_BREATHE, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_WARM_WHITE, NEO_OFF);
        neo_animation_alert.helper2 = 10;   // brightness steps
        neo_animation_alert.helper3 = PREFS_NEO_BRIGHTNESS+50; // max brightness
        startNeoAnim(&neo_animation_alert);
      break;
      case 'y':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_CYCLE8, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_PINK);
        startNeoAnim(&neo_animation_alert);
      break;
      case 'u':
        setNeoAnim(&neo_animation_alert, NEO_ANIM_CYCLE3, NEO_ANIM_ALERT);
        setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_PINK);
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

