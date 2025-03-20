// ----------------------------------
// ------------ engine --------------
// ----------------------------------

void updateServoAnimation() {
  
  // check if there's an animation happening currently
  // if not, do the home animation
  if(servo_animation_alert.active) {

    if(servo_animation_alert.function) {
      servo_animation_alert.function(&servo_animation_alert);
      updateAllServos();
      //Serial << "left mMillisForCompleteMove: " << wing_left.motor.mMillisForCompleteMove << endl;
    }

  } else {

    if(servo_animation_home.function) {
      servo_animation_home.function(&servo_animation_home);
      updateAllServos();
    }

  }
  
}


bool servoAnimationChecks(struct ServoAnimation *a) {

  if(!a) return false; // safety check

  // check that the animation delay is done. if not, wait again
  if(millis()-a->last_repeat < a->repeat_delay) {
    if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << "animation delay not done" << endl;
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
    if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << "animation done (time elapsed)" << endl;
    // callback anim done (time elapsed)
    callback_ServoAnimDone(a);
    return false;
  }

  // check that the number of repeats is complete
  if(a->repeat_count >= a->num_repeats 
     && a->num_repeats != -99) {
    a->start_time = -1;
    a->repeat_count = 0;
    a->active = false;
    if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << "animation done (num repeats)" << endl;
    // callback anim done (num repeats)
    callback_ServoAnimDone(a);
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
  if(a->start_time == -1) {
    a->repeat_count = 0;
    a->frame_index = 0;
    a->start_time = millis();
  }
  a->active = true;
  if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << "Servo animation: " << a->id;
  if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << " frame: " << a->frame_index+1 << "/" << a->num_frames << endl;

  if(wing_left.active == false || wing_right.active == false) {
    if(DEBUG_SERVO_ANIM == true && a->type != SERVO_ANIM_HOME) Serial << "restart servos (" << a->frame_index << ") " << endl;
    if(wing_left.active == false) {
      wing_left.motor.reattach();
      wing_left.active = true;
    }
    if(wing_right.active == false) {
      wing_right.motor.reattach();
      wing_right.active = true;
    }
    a->frame_index--;
    a->frame_delay = 0;
    a->last_frame = millis();
    return false; // fast forward to the next frame
  }

  return true;
}

// ----------------------------------



// ----------------------------------
// ----------- animations -----------
// ----------------------------------

void runServoAnim_none(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  // only 1 frame in this animation
  wing_left.motor.detach();
  wing_left.active = false;
  wing_right.motor.detach();
  wing_right.active = false;
  a->last_frame = millis();

}


void runServoAnim_gentle(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t flap_offset = a->helper1;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_HOME+flap_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_HOME-flap_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 250+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_sway(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t sway_offset = a->helper1;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_HOME-sway_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_HOME+sway_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_soaring(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t soaring_offset = a->helper1;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-soaring_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 40+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+soaring_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 40+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_touchgrass(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t down_offset = a->helper1;
  uint16_t down2_offset = a->helper2;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+down_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-down_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+down2_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-down2_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_swoosh(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t flap_offset = a->helper1;
  uint16_t down_offset = a->helper2;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP-flap_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP+flap_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 250+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+down_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-down_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_party(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t step1_offset = a->helper1;
  uint16_t step2_offset = a->helper2;
  uint16_t step3_offset = a->helper3;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_HOME, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_HOME, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP-step1_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-step1_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 2: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP-step2_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-step2_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 3: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP-step3_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-step3_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 4: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_UP-step1_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_DOWN-step1_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 5: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_HOME, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_HOME, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 6: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+step1_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP+step1_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 7: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+step2_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP+step2_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 8: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+step3_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP+step3_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 9: {
      Serial << "frame (" << a->frame_index << ") " << endl;
      wing_left.motor.setEaseTo(SERVO_LEFT_DOWN+step1_offset, velocity);
      wing_right.motor.setEaseTo(SERVO_RIGHT_UP+step1_offset, velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor.mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}

// ----------------------------------



// ----------------------------------
// --------- initialisers -----------
// ----------------------------------

void initServoAnim_none(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_NONE;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;

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

  a->function = runServoAnim_none;
}


void initServoAnim_gentle(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_GENTLE;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 40;

  a->num_frames = 2;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 300; // flap offset
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runServoAnim_gentle;
}


void initServoAnim_sway(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_SWAY;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 60;

  a->num_frames = 2;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 600; // sway offset
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runServoAnim_sway;
}


void initServoAnim_soaring(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_SOARING;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 40;

  a->num_frames = 2;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 200; // soaring offset
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runServoAnim_soaring;
}


void initServoAnim_touchgrass(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_TOUCHGRASS;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 70;

  a->num_frames = 2;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 200;     // down offset
  a->helper2 = 400;     // down2 offset
  a->helper3 = 0;

  a->function = runServoAnim_touchgrass;
}


void initServoAnim_swoosh(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_SWOOSH;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 50;

  a->num_frames = 2;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;       // flap offset
  a->helper2 = 250;     // down offset
  a->helper3 = 0;

  a->function = runServoAnim_swoosh;
}


void initServoAnim_party(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_PARTY;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 100;

  a->num_frames = 10;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 550;     // step1 offset
  a->helper2 = 300;     // step2 offset
  a->helper3 = 850;     // step3 offset

  a->function = runServoAnim_party;
}


void initServoAnimations() {
  // all servo animations are initialised when setServoAnim() is called
  initServoAnim_none(&servo_animation_home);
  servo_animation_home.type = SERVO_ANIM_HOME;
  initServoAnim_gentle(&servo_animation_alert);
  servo_animation_alert.type = SERVO_ANIM_ALERT;
}

// ----------------------------------


