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

    if(changed) {
      colorWipe( colourPalette[colour_index], 50 );
      changed = false;
    }

  }

}


void initNeopixels() {
  pixels.begin();
  pixels.show();
  pixels.setBrightness(PREFS_NEO_BRIGHTNESS);
  initNeoColours();
}

void initNeoColours() {
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