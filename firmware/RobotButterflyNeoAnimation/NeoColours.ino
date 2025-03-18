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

