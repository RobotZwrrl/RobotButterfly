/*
 * Robot Butterfly Neopixel Colours
 * --------------------------------
 * Colour palette for the neopixels
 * Calibrated at brightness = 10, some of the
 * colours may appear different at higher
 * brightnesses.
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



// ----------- neopixels -----------
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

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
  "White"
};

uint16_t pastelColours[NUM_PALETTE_COLOURS][3] = {
    {719,    170,  250},   // red
    {3561,   160,  250},   // orange
    {5701,   160,  250},   // golden yellow
    {9622,   170,  250},   // canary yellow
    {16545,  170,  250},   // green
    {32845,   60,  250},   // sky blue
    {43691,  120,  250},   // blue
    {27306,  120,  250},   // cyan
    {51613,  130,  250},   // purple
    {55613,   60,  250},   // lavendar
    {659,     90,  250},   // pink
    {63535,  140,  250},   // magenta
    {5461,   120,  250},   // warm white
    {8192,    90,  250},   // white
};

uint32_t colourPalette[NUM_PALETTE_COLOURS];

bool AUTO_ADVANCE = true;
long last_colour_change = 0;
uint8_t colour_index = 0;
uint8_t colour_index_prev = 0;
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
        if(DEBUG_NEO_COLOURS) Serial << pastelNames[colour_index] << " [" << colour_index << "]" << endl;
        updateNeopixels();
      break;
      case 'p':
        colour_index--;
        if(colour_index < 0) colour_index = NUM_PALETTE_COLOURS;
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



void updateNeopixels() {

  colour_index_prev = colour_index;

  if(AUTO_ADVANCE) {

    if(millis()-last_colour_change >= NEO_DEMO_ADVANCE) {
      colour_index++;
      if(colour_index >= NUM_PALETTE_COLOURS) colour_index = 0;
      if(DEBUG_NEO_COLOURS) Serial << pastelNames[colour_index] << " [" << colour_index << "]" << endl;
      colorWipe( colourPalette[colour_index], 50 );
      last_colour_change = millis();
    }

  } else {

    bool changed = false;
    if(colour_index_prev != colour_index) changed = true;

    if(changed) {
      colorWipe( colourPalette[colour_index], 50 );
    }

  }

}


void initNeopixels() {
  pixels.begin();
  pixels.show();
  pixels.setBrightness(PREFS_NEO_BRIGHTNESS);
  initColours();
}


void initColours() {
  for(uint8_t i=0; i<NUM_PALETTE_COLOURS; i++) {
    colourPalette[i] = pixels.gamma32( pixels.ColorHSV(pastelColours[i][0], pastelColours[i][1], pastelColours[i][2]) );
  }
}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

