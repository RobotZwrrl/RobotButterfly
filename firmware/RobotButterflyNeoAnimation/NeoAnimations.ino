// ----------------------------------
// ------------ engine --------------
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
    // callback anim done (time elapsed)
    callback_NeoAnimDone(a);
    return false;
  }

  // check that the number of repeats is complete
  if(a->repeat_count >= a->num_repeats 
     && a->num_repeats != -99) {
    a->start_time = -1;
    a->repeat_count = 0;
    a->active = false;
    if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "animation done (num repeats)" << endl;
    // callback anim done (num repeats)
    callback_NeoAnimDone(a);
    return false;
  }

  // increment the frame
  if(millis()-a->last_frame >= a->frame_delay) {
    a->frame_index++;
    if(a->frame_index > a->num_frames-1) {
      a->frame_index = 0;
      a->repeat_count++;
      a->last_repeat = millis();
      callback_NeoAnimLoop(a);
    }
  } else {
    return false;
  }

  // if it's here then the animation is active
  // first, reset the vars
  if(a->start_time == -1) { //   if(a->active == false) {
    a->repeat_count = 0;
    a->frame_index = 0;
    a->start_time = millis();
  }
  a->active = true;
  if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << "Neo animation: " << a->id;
  if(DEBUG_NEO_ANIMATION == true && a->type != NEO_ANIM_HOME) Serial << " frame: " << a->frame_index+1 << "/" << a->num_frames << endl;

  return true;

}

// ----------------------------------



// ----------------------------------
// ----------- animations -----------
// ----------------------------------

void runNeoAnim_none(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  // only 1 frame in this animation
  pixels.clear();
  for(uint8_t i=0; i<NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, colourPalette[NEO_OFF]);
  }
  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_squiggle(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  // do this each frame
  if(a->dir) {
    a->helper1++;
  } else {
    a->helper1--;
  }
  if(a->helper1 < (0+1)) {
    a->helper1 = (0+1);
    a->dir = !a->dir;
  }
  if(a->helper1 > (NEOPIXEL_COUNT-1-1)) {
    a->helper1 = (NEOPIXEL_COUNT-1-1);
    a->dir = !a->dir;
  }

  int p1 = a->helper1-1;
  int p2 = a->helper1;
  int p3 = a->helper1+1;

  // this happens every frame
  pixels.clear();
  for(uint8_t i=0; i<NEOPIXEL_COUNT; i++) {
    pixels.setPixelColor(i, colourPalette[NEO_OFF]);
  }
  pixels.setPixelColor(p1, colourPalette[a->colour_primary]);
  pixels.setPixelColor(p2, colourPalette[a->colour_secondary]);
  pixels.setPixelColor(p3, colourPalette[a->colour_primary]);
  pixels.show();
  a->last_frame = millis();
  // ---

}


void runNeoAnim_polkadot(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  switch(a->frame_index) {
    case 0: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, colourPalette[a->colour_primary]);
        } else {
          pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
        }
      }
    }
    break;
    case 1: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        if(i%2 == 0) {
          pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
        } else {
          pixels.setPixelColor(i, colourPalette[a->colour_primary]);
        }
      }
    }
    break;
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_range(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  int range = a->helper1;
  bool dir = a->dir;

  if(range > pixels.numPixels()) range = pixels.numPixels();
  if(range < 0) range = 0;

  if(dir) { // back to front
    
    for(uint8_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
    }
    for(uint8_t i=0; i<range; i++) {
      pixels.setPixelColor(i, colourPalette[a->colour_primary]);
    }

  } else { // front to back

    for(uint8_t i=pixels.numPixels()-1; i>0; i--) {
      pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
    }
    for(uint8_t i=pixels.numPixels()-1; i>(pixels.numPixels()-1-range); i--) {
      pixels.setPixelColor(i, colourPalette[a->colour_primary]);
    }

  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_funky(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  switch(a->frame_index) {
    case 0: {
      pixels.setPixelColor(0, colourPalette[a->colour_primary]);
      pixels.setPixelColor(1, colourPalette[a->colour_primary]);
      pixels.setPixelColor(2, colourPalette[a->colour_primary]);

      pixels.setPixelColor(3, colourPalette[a->colour_secondary]);
      pixels.setPixelColor(4, colourPalette[a->colour_secondary]);

      pixels.setPixelColor(5, colourPalette[a->colour_primary]);
      pixels.setPixelColor(6, colourPalette[a->colour_primary]);
      pixels.setPixelColor(7, colourPalette[a->colour_primary]);
    }
    break;
    case 1: {
      pixels.setPixelColor(0, colourPalette[a->colour_secondary]);
      pixels.setPixelColor(1, colourPalette[a->colour_secondary]);
      pixels.setPixelColor(2, colourPalette[a->colour_secondary]);

      pixels.setPixelColor(3, colourPalette[a->colour_primary]);
      pixels.setPixelColor(4, colourPalette[a->colour_primary]);

      pixels.setPixelColor(5, colourPalette[a->colour_secondary]);
      pixels.setPixelColor(6, colourPalette[a->colour_secondary]);
      pixels.setPixelColor(7, colourPalette[a->colour_secondary]);
    }
    break;
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_zwoop(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  int frame = a->frame_index;

  if(a->frame_index > 3) {
    frame = ((4*2)-1)-a->frame_index;
  }

  if(a->frame_index == 4) return; // skip duplicate
  if(a->frame_index == 7) return; // skip duplicate

  switch(frame) {
    case 0: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
      }
      pixels.setPixelColor(3, colourPalette[a->colour_primary]);
      pixels.setPixelColor(4, colourPalette[a->colour_primary]);
    }
    break;
    case 1: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
      }
      pixels.setPixelColor(2, colourPalette[a->colour_primary]);
      pixels.setPixelColor(5, colourPalette[a->colour_primary]);
    }
    break;
    case 2: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
      }
      pixels.setPixelColor(1, colourPalette[a->colour_primary]);
      pixels.setPixelColor(6, colourPalette[a->colour_primary]);
    }
    break;
    case 3: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
      }
      pixels.setPixelColor(0, colourPalette[a->colour_primary]);
      pixels.setPixelColor(7, colourPalette[a->colour_primary]);
    }
    break;
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_sprinkle(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  uint8_t place_a1;
  uint8_t place_a2;
  uint8_t place_a3;

  place_a1 = (uint8_t)random(0, NEOPIXEL_COUNT);
  place_a2 = (uint8_t)random(0, NEOPIXEL_COUNT);
  while(place_a2 == place_a1) {
    place_a2 = (uint8_t)random(0, NEOPIXEL_COUNT);
  }
  place_a3 = (uint8_t)random(0, NEOPIXEL_COUNT);
  while(place_a3 == place_a1 || place_a3 == place_a2) {
    place_a3 = (uint8_t)random(0, NEOPIXEL_COUNT);
  }

  uint8_t place_b1;
  uint8_t place_b2;

  place_b1 = (uint8_t)random(0, NEOPIXEL_COUNT);
  while(place_b1 == place_a1 || place_b1 == place_a2 || place_b1 == place_a3) {
    place_b1 = (uint8_t)random(0, NEOPIXEL_COUNT);
  }

  place_b2 = (uint8_t)random(0, NEOPIXEL_COUNT);
  while(place_b1 == place_b2 || place_b2 == place_a1 || place_b2 == place_a2 || place_b2 == place_a3) {
    place_b2 = (uint8_t)random(0, NEOPIXEL_COUNT);
  }

  pixels.setPixelColor(place_a1, colourPalette[a->colour_primary]);
  pixels.setPixelColor(place_a2, colourPalette[a->colour_primary]);
  pixels.setPixelColor(place_a3, colourPalette[a->colour_primary]);

  pixels.setPixelColor(place_b1, colourPalette[a->colour_secondary]);
  pixels.setPixelColor(place_b2, colourPalette[a->colour_secondary]);

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_rainbow(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  long pixel_hue = a->helper1*5;
  if(pixel_hue > 5*65536) {
    pixel_hue = 0;
    a->helper1 = 0;
  }

  pixels.rainbow(pixel_hue);

  a->helper1 += a->helper2;

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_rainbow_all(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  if(a->helper1 > (32767-100)) {
    a->helper1 = 0;
  }

  uint32_t color = pixels.ColorHSV(a->helper1*2, 250, 250);
  color = pixels.gamma32(color);

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
  }

  a->helper1 += a->helper2;

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_bounce(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
  }

  pixels.setPixelColor(a->helper1, colourPalette[a->colour_primary]);

  if(a->dir) {
    a->helper1++;
    if(a->helper1 >= pixels.numPixels()) {
      a->helper1 = pixels.numPixels()-1-1;
      a->dir = !a->dir;
    }
  } else {
    a->helper1--;
    if(a->helper1 <= 0) {
      a->helper1 = 0;
      a->dir = !a->dir;
    }
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_breathe(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, colourPalette[a->colour_primary]);
  }

  pixels.setBrightness(a->helper1);

  if(a->dir) {
    a->helper1 += a->helper2;
    if(a->helper1 >= a->helper3) {
      a->dir = !a->dir;
    }
  } else {
    a->helper1 -= a->helper2;
    if(a->helper1 <= 0) {
      a->dir = !a->dir;
      a->frame_index = a->num_frames-1-1; // push it to callback
    }
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_cycle8(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, colourPalette[a->colour_primary]);
  }

  for(uint8_t i=a->helper1; i<a->helper2; i++) {
    pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
  }

  if(a->helper2 >= pixels.numPixels()) {
    a->helper2 = pixels.numPixels();
    a->helper1++;
    if(a->helper1 >= pixels.numPixels()) {
      a->helper1 = 0;
      a->helper2 = 0;
      a->frame_index = a->num_frames-1-1; // push it to callback
    }
  } else {
    a->helper2++;
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_cycle3(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
  }

  pixels.setPixelColor(a->helper1-1, colourPalette[a->colour_primary]);
  pixels.setPixelColor(a->helper1, colourPalette[a->colour_primary]);
  pixels.setPixelColor(a->helper1+1, colourPalette[a->colour_primary]);

  pixels.setPixelColor(a->helper2-1, colourPalette[a->colour_primary]);
  pixels.setPixelColor(a->helper2, colourPalette[a->colour_primary]);
  pixels.setPixelColor(a->helper2+1, colourPalette[a->colour_primary]);

  a->helper1++;
  a->helper2++;

  if(a->helper1-3 >= pixels.numPixels()) {
    a->helper1 = -1;
    a->frame_index = a->num_frames-1-1; // push it to callback
  }

  if(a->helper2-3 >= pixels.numPixels()) {
    a->helper2 = -1;
  }

  pixels.show();
  a->last_frame = millis();

}


void runNeoAnim_ambiance(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  uint32_t paletteSpring[5] = { colourPalette[NEO_LAVENDER], colourPalette[NEO_CANARY_YELLOW], colourPalette[NEO_PINK], colourPalette[NEO_GREEN], colourPalette[NEO_LAVENDER] };
  uint32_t paletteSummer[5] = { colourPalette[NEO_GREEN], colourPalette[NEO_SKY_BLUE], colourPalette[NEO_CYAN], colourPalette[NEO_CANARY_YELLOW], colourPalette[NEO_GREEN] };
  uint32_t paletteAutumn[5] = { colourPalette[NEO_ORANGE], colourPalette[NEO_GOLDEN_YELLOW], colourPalette[NEO_RED], colourPalette[NEO_GOLDEN_YELLOW], colourPalette[NEO_ORANGE] };
  uint32_t paletteWinter[5] = { colourPalette[NEO_BLUE], colourPalette[NEO_WHITE], colourPalette[NEO_SKY_BLUE], colourPalette[NEO_WHITE], colourPalette[NEO_BLUE] };
  uint32_t paletteTuttiFruiti[5] = { colourPalette[NEO_MAGENTA], colourPalette[NEO_CYAN], colourPalette[NEO_GOLDEN_YELLOW], colourPalette[NEO_PURPLE], colourPalette[NEO_MAGENTA] };

  pixels.clear();

  for(uint8_t i=0; i<pixels.numPixels(); i++) {
    if(a->helper1 == 0) pixels.setPixelColor(i, paletteSpring[a->helper3]);
    if(a->helper1 == 1) pixels.setPixelColor(i, paletteSummer[a->helper3]);
    if(a->helper1 == 2) pixels.setPixelColor(i, paletteAutumn[a->helper3]);
    if(a->helper1 == 3) pixels.setPixelColor(i, paletteWinter[a->helper3]);
    if(a->helper1 == 4) pixels.setPixelColor(i, paletteTuttiFruiti[a->helper3]);
  }

  for(uint8_t i=0; i<a->helper2; i++) {
    if(a->helper1 == 0) pixels.setPixelColor(i, paletteSpring[a->helper3+1]);
    if(a->helper1 == 1) pixels.setPixelColor(i, paletteSummer[a->helper3+1]);
    if(a->helper1 == 2) pixels.setPixelColor(i, paletteAutumn[a->helper3+1]);
    if(a->helper1 == 3) pixels.setPixelColor(i, paletteWinter[a->helper3+1]);
    if(a->helper1 == 4) pixels.setPixelColor(i, paletteTuttiFruiti[a->helper3+1]);
  }

  if(a->helper2 >= pixels.numPixels()-1) {
    a->helper2 = 0;
    a->helper3++; // colour index
    if(a->helper3+1 >= 5) {
      a->helper3 = 0;
      a->frame_index = a->num_frames-1-1; // push it to callback
    }
  } else {
    a->helper2++;
  }

  pixels.show();
  a->last_frame = millis();

}


// this is a template for copying and pasting
void runNeoAnim_template(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  switch(a->frame_index) {
    case 0: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_primary]);
      }
    }
    break;
    case 1: {
      for(uint8_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, colourPalette[a->colour_secondary]);
      }
    }
    break;
  }

  pixels.show();
  a->last_frame = millis();

}

// ----------------------------------



// ----------------------------------
// --------- initialisers -----------
// ----------------------------------

void initNeoAnim_none(struct NeoAnimation *a) {
  a->id = NEO_ANIM_NONE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = 1;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = 80;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_none;
}


void initNeoAnim_polkadot(struct NeoAnimation *a) {
  a->id = NEO_ANIM_POLKADOT;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 2;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_polkadot;
}


void initNeoAnim_squiggle(struct NeoAnimation *a) {
  a->id = NEO_ANIM_SQUIGGLE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 12;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_squiggle;
}


void initNeoAnim_range(struct NeoAnimation *a) {
  a->id = NEO_ANIM_RANGE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_range;
}


void initNeoAnim_funky(struct NeoAnimation *a) {
  a->id = NEO_ANIM_FUNKY;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 2;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_funky;
}


void initNeoAnim_zwoop(struct NeoAnimation *a) {
  a->id = NEO_ANIM_ZWOOP;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 8;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_zwoop;
}


void initNeoAnim_sprinkle(struct NeoAnimation *a) {
  a->id = NEO_ANIM_SPRINKLE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_sprinkle;
}


void initNeoAnim_rainbow(struct NeoAnimation *a) {
  a->id = NEO_ANIM_RAINBOW;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 15545;  // hue counter start on green
  a->helper2 = 150;    // hue steps
  a->helper3 = 0;

  a->function = runNeoAnim_rainbow;
}


void initNeoAnim_rainbow_all(struct NeoAnimation *a) {
  a->id = NEO_ANIM_RAINBOW_ALL;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 15545;   // hue counter start on green (appears as blue though lol)
  a->helper2 = 500;     // hue steps
  a->helper3 = 0;

  a->function = runNeoAnim_rainbow_all;
}


void initNeoAnim_bounce(struct NeoAnimation *a) {
  a->id = NEO_ANIM_BOUNCE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_bounce;
}


void initNeoAnim_breathe(struct NeoAnimation *a) {
  a->id = NEO_ANIM_BREATHE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 255;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 500; // dwell time when faded out
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 1;  // brightness steps
  a->helper3 = PREFS_NEO_BRIGHTNESS;  // max brightness

  a->function = runNeoAnim_breathe;
}


void initNeoAnim_cycle8(struct NeoAnimation *a) {
  a->id = NEO_ANIM_CYCLE8;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 255;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;  // trailing pixel
  a->helper2 = 0;  // leading pixel
  a->helper3 = 0;

  a->function = runNeoAnim_cycle8;
}


void initNeoAnim_cycle3(struct NeoAnimation *a) {
  a->id = NEO_ANIM_CYCLE3;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 255;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 1;
  a->helper2 = 7;
  a->helper3 = 0;

  a->function = runNeoAnim_cycle3;
}


void initNeoAnim_ambiance(struct NeoAnimation *a) {
  a->id = NEO_ANIM_AMBIANCE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 255;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;  // colour palette
  a->helper2 = 0;  // leading pixel
  a->helper3 = 0;  // colour index

  a->function = runNeoAnim_ambiance;
}


void initNeoAnimations() {
  initNeoAnim_none(&neo_animation_home);
  neo_animation_home.type = NEO_ANIM_HOME;
  initNeoAnim_none(&neo_animation_alert);
  neo_animation_alert.type = NEO_ANIM_ALERT;
}

// ----------------------------------

