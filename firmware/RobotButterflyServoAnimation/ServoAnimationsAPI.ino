// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the servo animation is done
void callback_ServoAnimDone(struct ServoAnimation *a) {
  if(a->type == SERVO_ANIM_HOME) return;
  Serial << "Callback: Servo animation (" << a->id << ") done" << endl;
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
    case SERVO_ANIM_GENTLE:
      initServoAnim_gentle(a);
    break;
    case SERVO_ANIM_SWAY:
      initServoAnim_sway(a);
    break;
    case SERVO_ANIM_SOARING:
      initServoAnim_soaring(a);
    break;
    case SERVO_ANIM_TOUCHGRASS:
      initServoAnim_touchgrass(a);
    break;
    case SERVO_ANIM_SWOOSH:
      initServoAnim_swoosh(a);
    break;
    case SERVO_ANIM_PARTY:
      initServoAnim_party(a);
    break;
    case SERVO_ANIM_FLUTTER:
      initServoAnim_flutter(a);
    break;
    case SERVO_ANIM_RANGE:
      initServoAnim_range(a);
    break;
    case SERVO_ANIM_POSITION:
      initServoAnim_position(a);
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
  callback_ServoAnimDone(a);
}
// ----------------------------------


// ----------------------------------
// ------------ getters -------------
// ----------------------------------

int servoSafeValLeft(int n) {
  int val = n;

  if(n < SERVO_LEFT_DOWN) val = SERVO_LEFT_DOWN;
  if(n > SERVO_LEFT_UP) val = SERVO_LEFT_UP;

  return val;
}

int servoSafeValRight(int n) {
  int val = n;

  if(n > SERVO_RIGHT_DOWN) val = SERVO_RIGHT_DOWN;
  if(n < SERVO_RIGHT_UP) val = SERVO_RIGHT_UP;

  return val;
}

// ----------------------------------
