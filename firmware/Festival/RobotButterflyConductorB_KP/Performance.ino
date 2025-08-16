void neoAnimationProximity(uint8_t n) {
  setNeoAnim(&neo_animation_home, NEO_ANIM_RANGE, NEO_ANIM_HOME);
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_home, NEO_RED, NEO_OFF);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_home, NEO_BLUE, NEO_OFF);
  } else { // unknown
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_OFF);
  }
  setNeoAnimRangeVal(&neo_animation_home, n);
  startNeoAnim(&neo_animation_home);
}


void restAction() {
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
  setServoAnimDuration(&servo_animation_alert, 3000);
  startServoAnim(&servo_animation_alert);
}


void restNeo() {
  setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_home, NEO_RED, NEO_WHITE);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_home, NEO_BLUE, NEO_WHITE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimDuration(&neo_animation_home, 500);
  startNeoAnim(&neo_animation_home);
}

