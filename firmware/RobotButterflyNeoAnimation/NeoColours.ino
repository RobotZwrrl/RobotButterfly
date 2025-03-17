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

