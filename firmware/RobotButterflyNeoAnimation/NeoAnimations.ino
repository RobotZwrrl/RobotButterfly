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

// ----------------------------------



// ----------------------------------
// ----------- animations -----------
// ----------------------------------

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


void initNeoAnimations() {
  initNeoAnim_none(&neo_animation_home);
  neo_animation_home.type = NEO_ANIM_HOME;
  initNeoAnim_none(&neo_animation_alert);
  neo_animation_alert.type = NEO_ANIM_ALERT;
}

// ----------------------------------

