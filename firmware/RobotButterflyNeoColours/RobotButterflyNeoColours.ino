/*
 * Robot Butterfly Neopixel Patterns
 * --------------------------------
 * Blinky patterns with the colour palette
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 11 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
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
bool changed = false;
// -----------------------------------



void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initNeopixels();

  Serial << "Ready" << endl;
}


void loop() {

  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateNeopixels();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'a':
        AUTO_ADVANCE = !AUTO_ADVANCE;
        if(AUTO_ADVANCE) {
          if(DEBUG_NEO_COLOURS) Serial << "AUTO_ADVANCE ON" << endl;
        } else {
          if(DEBUG_NEO_COLOURS) Serial << "AUTO_ADVANCE OFF" << endl;
        }
      break;
      case 'n':
        colour_index++;
        if(colour_index >= NUM_PALETTE_COLOURS) colour_index = 0;
        changed = true;
        if(DEBUG_NEO_COLOURS) Serial << pastelNames[colour_index] << " [" << colour_index << "]" << endl;
        updateNeopixels();
      break;
      case 'p':
        colour_index--;
        if(colour_index < 0) colour_index = NUM_PALETTE_COLOURS;
        changed = true;
        if(DEBUG_NEO_COLOURS) Serial << pastelNames[colour_index] << " [" << colour_index << "]" << endl;
        updateNeopixels();
      break;
      case 'h':
        Serial << "a: toggle AUTO_ADVANCE" << endl;
        Serial << "n: next colour" << endl;
        Serial << "p: prev colour" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}

