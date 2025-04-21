// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the neo animation is done
void callback_NeoAnimDone(struct NeoAnimation *a) {
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") done" << endl;
}

// the neo animation is done a loop
void callback_NeoAnimLoop(struct NeoAnimation *a) {
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") looped" << endl;
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
  callback_NeoAnimDone(a);
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

