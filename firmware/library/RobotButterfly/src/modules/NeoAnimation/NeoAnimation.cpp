#include "modules/NeoAnimation/NeoAnimation.h"

NeoCallback onNeoAnimDoneCallback = NULL;
NeoCallback onNeoAnimLoopCallback = NULL;

NeoAnimation neo_animation_home;
NeoAnimation neo_animation_alert;

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
void runNeoAnim_ambiance(struct NeoAnimation *animation);
void runNeoAnim_uno(struct NeoAnimation *animation);


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
    if(onNeoAnimDoneCallback) onNeoAnimDoneCallback(a);
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
    if(onNeoAnimDoneCallback) onNeoAnimDoneCallback(a);
    return false;
  }

  // increment the frame
  if(millis()-a->last_frame >= a->frame_delay) {
    a->frame_index++;
    if(a->frame_index > a->num_frames-1) {
      a->frame_index = 0;
      a->repeat_count++;
      a->last_repeat = millis();
      if(onNeoAnimLoopCallback) onNeoAnimLoopCallback(a);
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
    if(a->helper1 == NEO_ANIM_AMBIANCE_SPRING) pixels.setPixelColor(i, paletteSpring[a->helper3]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_SUMMER) pixels.setPixelColor(i, paletteSummer[a->helper3]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_AUTUMN) pixels.setPixelColor(i, paletteAutumn[a->helper3]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_WINTER) pixels.setPixelColor(i, paletteWinter[a->helper3]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_TUTTI_FRUITI) pixels.setPixelColor(i, paletteTuttiFruiti[a->helper3]);
  }

  for(uint8_t i=0; i<a->helper2; i++) {
    if(a->helper1 == NEO_ANIM_AMBIANCE_SPRING) pixels.setPixelColor(i, paletteSpring[a->helper3+1]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_SUMMER) pixels.setPixelColor(i, paletteSummer[a->helper3+1]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_AUTUMN) pixels.setPixelColor(i, paletteAutumn[a->helper3+1]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_WINTER) pixels.setPixelColor(i, paletteWinter[a->helper3+1]);
    if(a->helper1 == NEO_ANIM_AMBIANCE_TUTTI_FRUITI) pixels.setPixelColor(i, paletteTuttiFruiti[a->helper3+1]);
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


void runNeoAnim_uno(struct NeoAnimation *a) {

  if(!neoAnimationChecks(a)) return;

  pixels.clear();

  int uno = a->helper1;
  if(uno < 0) uno = 0;
  if(uno > pixels.numPixels()) uno = pixels.numPixels()-1;
  if(a->helper1 == -99) uno = -99;

  int duo = a->helper2;
  if(duo < 0) duo = 0;
  if(duo > pixels.numPixels()) duo = pixels.numPixels()-1;
  if(a->helper2 == -99) duo = -99;

  switch(a->frame_index) {
    case 0: {
      if(uno != -99) pixels.setPixelColor(uno, colourPalette[a->colour_primary]);
      if(duo != -99) pixels.setPixelColor(duo, colourPalette[a->colour_primary]);
    }
    break;
    case 1: {
      if(uno != -99) pixels.setPixelColor(uno, colourPalette[a->colour_secondary]);
      if(duo != -99) pixels.setPixelColor(duo, colourPalette[a->colour_secondary]);
    }
    break;
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


void initNeoAnim_uno(struct NeoAnimation *a) {
  a->id = NEO_ANIM_UNO;
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
  a->helper1 = -99; // index of pixel 1
  a->helper2 = -99; // index of pixel 2
  a->helper3 = 0;

  a->function = runNeoAnim_uno;
}


void initNeoAnimations() {

  onNeoAnimDoneCallback = NULL;
  onNeoAnimLoopCallback = NULL;

  initNeoAnim_none(&neo_animation_home);
  neo_animation_home.type = NEO_ANIM_HOME;
  initNeoAnim_none(&neo_animation_alert);
  neo_animation_alert.type = NEO_ANIM_ALERT;

  if(RTOS_ENABLED) {

    Mutex_NEOANIM = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_NEOANIM_code,     // task function
                      "Task_NEOANIM",        // name of task
                      STACK_NEOANIM,         // stack size of task
                      NULL,                  // parameter of the task
                      PRIORITY_NEOANIM_MID,  // priority of the task (low number = low priority)
                      &Task_NEOANIM,         // task handle to keep track of created task
                      TASK_CORE_NEOANIM);    // pin task to core
    
  }

}

// ----------------------------------


// ----------------------------------
// ------------ setters -------------
// ----------------------------------

// params: neo animation, id of animation, type
void setNeoAnim(struct NeoAnimation *a, uint8_t n, uint8_t t) {
  
  pixels.setBrightness(PREFS_NEO_BRIGHTNESS); // reset the brightness

  // init
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
    case NEO_ANIM_RANGE:
      initNeoAnim_range(a);
    break;
    case NEO_ANIM_FUNKY:
      initNeoAnim_funky(a);
    break;
    case NEO_ANIM_ZWOOP:
      initNeoAnim_zwoop(a);
    break;
    case NEO_ANIM_SPRINKLE:
      initNeoAnim_sprinkle(a);
    break;
    case NEO_ANIM_RAINBOW:
      initNeoAnim_rainbow(a);
    break;
    case NEO_ANIM_RAINBOW_ALL:
      initNeoAnim_rainbow_all(a);
    break;
    case NEO_ANIM_BOUNCE:
      initNeoAnim_bounce(a);
    break;
    case NEO_ANIM_BREATHE:
      initNeoAnim_breathe(a);
    break;
    case NEO_ANIM_CYCLE8:
      initNeoAnim_cycle8(a);
    break;
    case NEO_ANIM_CYCLE3:
      initNeoAnim_cycle3(a);
    break;
    case NEO_ANIM_AMBIANCE:
      initNeoAnim_ambiance(a);
    break;
    case NEO_ANIM_UNO:
      initNeoAnim_uno(a);
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
  if(onNeoAnimDoneCallback) onNeoAnimDoneCallback(a);
}

// params: neo animation, direction
void setNeoAnimDir(struct NeoAnimation *a, bool dir) {
  a->dir = dir;
}

// params: neo animation, val for number of leds lit
void setNeoAnimRangeVal(struct NeoAnimation *a, int val) {
  a->helper1 = val;
}

// params: neo animation, val for number of steps (larger = faster)
void setNeoAnimRainbowSteps(struct NeoAnimation *a, int val) {
  a->helper2 = val;
}

// params: neo animation, val for number of steps (larger = faster)
void setNeoAnimBreatheSteps(struct NeoAnimation *a, int val) {
  a->helper2 = val;
}

// params: neo animation, val for max brightness which can be more than prefs
void setNeoAnimBreatheMaxBrightness(struct NeoAnimation *a, int val) {
  a->helper3 = val;
}

// params: neo animation, id of the ambiance animation - see enum neoAnimAmbianceID
void setNeoAnimAmbiance(struct NeoAnimation *a, int id) {
  a->helper1 = id;
}

// params: neo animation, index of the lit pixel
void setNeoAnimUno(struct NeoAnimation *a, int uno) {
  a->helper1 = uno;
}

// params: neo animation, index of the lit pixel
void setNeoAnimDuo(struct NeoAnimation *a, int duo) {
  a->helper2 = duo;
}
// ----------------------------------


// ----------------------------------
// ---------- neo colours -----------
// ----------------------------------

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

uint32_t colourFromHSV(uint16_t hue, uint16_t sat, uint16_t val) {
  return pixels.gamma32( pixels.ColorHSV(hue, sat, val) );
}
// ----------------------------------


void Task_NEOANIM_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_NEOANIM, (TickType_t)10) == pdTRUE) {
      
      updateNeoAnimation();

      if(DEBUG_NEOANIM_RTOS == true && millis()-last_neoanim_rtos_print >= 1000) {
        Serial << "neoanim stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
        last_neoanim_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_NEOANIM);
    }
    
    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_NEOANIM != 0) vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_NEOANIM );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setNeoAnimationTaskPriority(uint8_t p) {
  
  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_NEOANIM);
  vTaskPrioritySet(Task_NEOANIM, p);
  if (DEBUG_NEOANIM_RTOS) Serial << "changed NEOANIM task priority - new: " << p << " prev: " << prev_priority << endl;

}

