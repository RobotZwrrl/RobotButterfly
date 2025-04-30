// ----------------------------------
// ----------- callbacks ------------
// ----------------------------------

// the servo animation is done
void servoAnimDoneCallback(struct ServoAnimation *a) {
  if(a->type == SERVO_ANIM_HOME) return;
  Serial << "Callback: Servo animation (" << a->id << ") done" << endl;
}

// the servo animation has looped
void servoAnimLoopCallback(struct ServoAnimation *a) {
  if(a->type == SERVO_ANIM_HOME) return;
  if(a->num_frames <= 1) return; // skip the one frame animations
  Serial << "Callback: Servo animation (" << a->id << ") looped" << endl;
}

// ----------------------------------


// ----------------------------------
// ------------ setters -------------
// ----------------------------------

// params: Servo animation, id of animation, type
void setServoAnim(struct ServoAnimation *a, uint8_t n, uint8_t t) {
  
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

// params: servo animation, duration of animation
void setServoAnimDuration(struct ServoAnimation *a, long duration) {
  a->duration = duration;
}

// params: servo animation, num repeats of animation
void setServoAnimRepeats(struct ServoAnimation *a, int r) {
  a->num_repeats = r;
}

// params: servo animation, velocity (degrees per second)
void setServoAnimSpeed(struct ServoAnimation *a, uint16_t del) {
  a->velocity = del;
}

// params: servo animation
void startServoAnim(struct ServoAnimation *a) {
  a->active = true;
}

// params: servo animation
void stopServoAnim(struct ServoAnimation *a) {
  a->active = false;
  if(onServoAnimDoneCallback) onServoAnimDoneCallback(a);
}

// params: servo animation, range
void setServoAnimRange(struct ServoAnimation *a, int n) {
  a->helper2 = n;
}

// params: servo animation, wings to flutter - see enum servoAnimFlutterWings
void setServoAnimFlutterWings(struct ServoAnimation *a, int n) {
  a->helper1 = n;
}

// params: servo animation, wings flutter position - see enum servoAnimFlutterPos
void setServoAnimFlutterPos(struct ServoAnimation *a, int n) {
  a->helper2 = n;
}

// params: servo animation, flutter offset (larger number = bigger movement)
void setServoAnimFlutterOffset(struct ServoAnimation *a, int n) {
  a->helper3 = n;
}

// params: servo animation, span for the range val - see enum servoAnimRangeSpan
void setServoAnimRangeSpan(struct ServoAnimation *a, int n) {
  a->helper1 = n;
}

// params: servo animation, range 0-9 inclusive
void setServoAnimRangeVal(struct ServoAnimation *a, int n) {
  a->helper2 = n;
}

// params: servo animation, position - see enum servoAnimPosition
void setServoAnimPositionLeft(struct ServoAnimation *a, int n) {
  a->helper1 = n;
}

// params: servo animation, position - see enum servoAnimPosition
void setServoAnimPositionRight(struct ServoAnimation *a, int n) {
  a->helper2 = n;
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
