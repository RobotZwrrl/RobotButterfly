// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the alert animation is done
void callback_ServoAnimAlertDone(struct ServoAnimation *a, uint8_t type) {
  //setServoAnimColours(Servo_GOLDEN_YELLOW, Servo_SKY_BLUE);
}

// the home animation is done
void callback_ServoAnimHomeDone(struct ServoAnimation *a, uint8_t type) {
  //setServoAnimColours(Servo_OFF, Servo_OFF);
}

// ----------------------------------


// ----------------------------------
// ------------ setters -------------
// ----------------------------------

// params: Servo animation, name of animation, type
void setServoAnim(struct ServoAnimation *a, uint8_t n, uint8_t t) {
  
  // init
  // TODO - all of the Servo animations
  switch(n) {
    case SERVO_ANIM_NONE:
      initServoAnim_none(a);
    break;
    case SERVO_ANIM_GENTLEFLAP:
      initServoAnim_gentleflap(a);
    break;
  }

  a->type = t;
}

// params: Servo animation, duration of animation
void setServoAnimDuration(struct ServoAnimation *a, long duration) {
  a->duration = duration;
}

// params: Servo animation, num repeats of animation
void setServoAnimRepeats(struct ServoAnimation *a, int r) {
  a->num_repeats = r;
}

// params: Servo animation, velocity (degrees per second)
void setServoAnimSpeed(struct ServoAnimation *a, uint16_t del) {
  a->velocity = del;
}

// params: Servo animation
void startServoAnim(struct ServoAnimation *a) {
  a->active = true;
}

// params: Servo animation
void stopServoAnim(struct ServoAnimation *a) {
  a->active = false;
}
// ----------------------------------
