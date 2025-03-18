// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the alert animation is done
void callback_NeoAnimAlertDone(struct NeoAnimation *a, uint8_t type) {
  //setNeoAnimColours(NEO_GOLDEN_YELLOW, NEO_SKY_BLUE);
}

// the home animation is done
void callback_NeoAnimHomeDone(struct NeoAnimation *a, uint8_t type) {
  //setNeoAnimColours(NEO_OFF, NEO_OFF);
}

// ----------------------------------


// ----------------------------------
// ------------ setters -------------
// ----------------------------------

// params: neo animation, name of animation, type
void setNeoAnim(struct NeoAnimation *a, uint8_t n, uint8_t t) {
  
  // init
  // TODO - all of the neo animations
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
}
// ----------------------------------

