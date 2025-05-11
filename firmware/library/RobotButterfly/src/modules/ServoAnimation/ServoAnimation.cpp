#include "modules/ServoAnimation/ServoAnimation.h"
#include "ServoEasing.hpp"

ServoAnimCallback onServoAnimDoneCallback = NULL;
ServoAnimCallback onServoAnimLoopCallback = NULL;

ServoAnimation servo_animation_home;
ServoAnimation servo_animation_alert;

ServoEasing s1;
ServoEasing s2;

void runServoAnim_none(struct ServoAnimation *animation);
void runServoAnim_gentle(struct ServoAnimation *animation);
void runServoAnim_sway(struct ServoAnimation *animation);
void runServoAnim_soaring(struct ServoAnimation *animation);
void runServoAnim_touchgrass(struct ServoAnimation *animation);
void runServoAnim_swoosh(struct ServoAnimation *animation);
void runServoAnim_party(struct ServoAnimation *animation);
void runServoAnim_flutter(struct ServoAnimation *animation);
void runServoAnim_range(struct ServoAnimation *animation);
void runServoAnim_position(struct ServoAnimation *animation);


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
    if(onServoAnimDoneCallback) onServoAnimDoneCallback(a);
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
    if(onServoAnimDoneCallback) onServoAnimDoneCallback(a);
    return false;
  }

  // increment the frame
  if(millis()-a->last_frame >= a->frame_delay) {
    a->frame_index++;
    if(a->frame_index > a->num_frames-1) {
      a->frame_index = 0;
      a->repeat_count++;
      a->last_repeat = millis();
      if(onServoAnimLoopCallback) onServoAnimLoopCallback(a);
    }
    if(DEBUG_SERVO_ANIM) Serial << "frame (" << a->frame_index << ") " << endl;
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
      wing_left.motor->reattach();
      wing_left.active = true;
    }
    if(wing_right.active == false) {
      wing_right.motor->reattach();
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
  wing_left.motor->detach();
  wing_left.active = false;
  wing_right.motor->detach();
  wing_right.active = false;
  a->last_frame = millis();

}


void runServoAnim_gentle(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t flap_offset = a->helper1;
  uint8_t velocity = a->velocity;

  switch(a->frame_index) {
    case 0: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_HOME+flap_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_HOME-flap_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 250+wing_left.motor->mMillisForCompleteMove;
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
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_HOME-sway_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_HOME+sway_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor->mMillisForCompleteMove;
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
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-soaring_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 40+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+soaring_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 40+wing_left.motor->mMillisForCompleteMove;
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
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+down_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-down_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+down2_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-down2_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 80+wing_left.motor->mMillisForCompleteMove;
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
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP-flap_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP+flap_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 250+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+down_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-down_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 100+wing_left.motor->mMillisForCompleteMove;
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
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_HOME), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_HOME), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 1: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP-step1_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-step1_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 2: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP-step2_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-step2_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 3: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP-step3_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-step3_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 4: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_UP-step1_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_DOWN-step1_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 5: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_HOME), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_HOME), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 6: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+step1_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP+step1_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 7: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+step2_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP+step2_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 8: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+step3_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP+step3_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
    case 9: {
      wing_left.motor->setEaseTo( getServoSafeValLeft(SERVO_LEFT_DOWN+step1_offset), velocity);
      wing_right.motor->setEaseTo( getServoSafeValRight(SERVO_RIGHT_UP+step1_offset), velocity);
      synchronizeAllServosAndStartInterrupt(false);
      a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
      a->last_frame = millis();
    }
    break;
  }

}


void runServoAnim_flutter(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t main = a->helper1;
  uint16_t alt = a->helper2;
  int flutter_offset = (int)a->helper3;
  uint8_t velocity = a->velocity;

  int sign = 1;
  int left_val = -99;
  int right_val = -99;
  
  if(a->frame_index%2 == 0) {
    sign = 1;
  } else {
    sign = -1;
  }

  switch(main) {
    case SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME: {  // both home
      
      if(sign == 1) {
        left_val = SERVO_LEFT_HOME+(2*flutter_offset);
        right_val = SERVO_RIGHT_HOME-(2*flutter_offset);
      } else {
        left_val = SERVO_LEFT_HOME;
        right_val = SERVO_RIGHT_HOME;
      }

    }
    break;
    case SERVO_ANIM_FLUTTER_WINGS_BOTH_UP: {  // both up
      left_val = SERVO_LEFT_UP-(sign*2*flutter_offset);
      right_val = SERVO_RIGHT_UP+(sign*2*flutter_offset);
    }
    break;
    case SERVO_ANIM_FLUTTER_WINGS_LEFT_HOME: {  // left home

      if(sign == 1) {
        left_val = SERVO_LEFT_HOME+(2*flutter_offset);
      } else {
        left_val = SERVO_LEFT_HOME;
      }

      if(alt == SERVO_ANIM_FLUTTER_POS_HOME) { // alt home
        right_val = SERVO_RIGHT_HOME;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_UP) { // alt up
        right_val = SERVO_RIGHT_UP;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_DOWN) { // alt down
        right_val = SERVO_RIGHT_DOWN;
      }

    }
    break;
    case SERVO_ANIM_FLUTTER_WINGS_LEFT_UP: {  // left up

      left_val = SERVO_LEFT_UP-(sign*2*flutter_offset);

      if(alt == SERVO_ANIM_FLUTTER_POS_HOME) { // alt home
        right_val = SERVO_RIGHT_HOME;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_UP) { // alt up
        right_val = SERVO_RIGHT_UP;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_DOWN) { // alt down
        right_val = SERVO_RIGHT_DOWN;
      }

    }
    break;
    case SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME: {  // right home

      if(sign == 1) {
        right_val = SERVO_RIGHT_HOME-(2*flutter_offset);
      } else {
        right_val = SERVO_RIGHT_HOME;
      }

      if(alt == SERVO_ANIM_FLUTTER_POS_HOME) { // alt home
        left_val = SERVO_LEFT_HOME;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_UP) { // alt up
        left_val = SERVO_LEFT_UP;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_DOWN) { // alt down
        left_val = SERVO_LEFT_DOWN;
      }

    }
    break;
    case SERVO_ANIM_FLUTTER_WINGS_RIGHT_UP: {  // right up

      right_val = SERVO_RIGHT_UP+(sign*2*flutter_offset);

      if(alt == SERVO_ANIM_FLUTTER_POS_HOME) { // alt home
        left_val = SERVO_LEFT_HOME;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_UP) { // alt up
        left_val = SERVO_LEFT_UP;
      } else if(alt == SERVO_ANIM_FLUTTER_POS_DOWN) { // alt down
        left_val = SERVO_LEFT_DOWN;
      }

    }
    break;
  }


  // -- same for both frames --

  if(left_val != -99) wing_left.motor->setEaseTo( getServoSafeValLeft(left_val), velocity);
  if(right_val != -99) wing_right.motor->setEaseTo( getServoSafeValRight(right_val), velocity);

  synchronizeAllServosAndStartInterrupt(false);
  if(left_val != -99) a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
  if(right_val != -99) a->frame_delay = 0+wing_right.motor->mMillisForCompleteMove;
  a->last_frame = millis();

  // --

}


void runServoAnim_range(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t range = a->helper1;
  uint16_t val = a->helper2;
  uint8_t velocity = a->velocity;

  int left_val = -99;
  int right_val = -99;
  
  switch(range) {
    case SERVO_ANIM_RANGE_DOWN_UP: { // down to up
      left_val = map(val, 0, 9, SERVO_LEFT_DOWN, SERVO_LEFT_UP);
      right_val = map(val, 0, 9, SERVO_RIGHT_DOWN, SERVO_RIGHT_UP);
    }
    break;
    case SERVO_ANIM_RANGE_HOME_UP: { // home to up
      left_val = map(val, 0, 9, SERVO_LEFT_HOME, SERVO_LEFT_UP);
      right_val = map(val, 0, 9, SERVO_RIGHT_HOME, SERVO_RIGHT_UP);
    }
    break;
    case SERVO_ANIM_RANGE_ALT_HOME_UP: { // alt
      left_val = map(val, 0, 9, SERVO_LEFT_HOME, SERVO_LEFT_UP);
      right_val = map(val, 0, 9, SERVO_RIGHT_UP, SERVO_RIGHT_HOME);
    }
    break;
  }

  // -- only one frame --

  if(left_val != -99) wing_left.motor->setEaseTo( getServoSafeValLeft(left_val), velocity);
  if(right_val != -99) wing_right.motor->setEaseTo( getServoSafeValRight(right_val), velocity);

  synchronizeAllServosAndStartInterrupt(false);
  if(left_val != -99) a->frame_delay = 0+wing_left.motor->mMillisForCompleteMove;
  if(right_val != -99) a->frame_delay = 0+wing_right.motor->mMillisForCompleteMove;
  a->last_frame = millis();

  // --

}


void runServoAnim_position(struct ServoAnimation *a) {

  if(!servoAnimationChecks(a)) return;

  uint16_t pos_L = a->helper1;
  uint16_t pos_R = a->helper2;
  uint8_t velocity = a->velocity;

  int left_val = -99;
  int right_val = -99;

  int del_L = 0;
  int del_R = 0;

  if(pos_L == SERVO_ANIM_POSITION_DOWN) left_val = SERVO_LEFT_DOWN;
  if(pos_L == SERVO_ANIM_POSITION_HOME) left_val = SERVO_LEFT_HOME;
  if(pos_L == SERVO_ANIM_POSITION_UP) left_val = SERVO_LEFT_UP;

  if(pos_R == SERVO_ANIM_POSITION_DOWN) right_val = SERVO_RIGHT_DOWN;
  if(pos_R == SERVO_ANIM_POSITION_HOME) right_val = SERVO_RIGHT_HOME;
  if(pos_R == SERVO_ANIM_POSITION_UP) right_val = SERVO_RIGHT_UP;

  // -- only one frame --

  if(left_val != -99) wing_left.motor->setEaseTo( getServoSafeValLeft(left_val), velocity);
  if(right_val != -99) wing_right.motor->setEaseTo( getServoSafeValRight(right_val), velocity);

  synchronizeAllServosAndStartInterrupt(false);
  if(left_val != -99) del_L = 0+wing_left.motor->mMillisForCompleteMove;
  if(right_val != -99) del_R = 0+wing_right.motor->mMillisForCompleteMove;
  if(del_L > del_R) {
    a->frame_delay = del_L;
  } else {
    a->frame_delay = del_R;
  }
  a->last_frame = millis();

  // --

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


void initServoAnim_flutter(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_FLUTTER;
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
  a->helper1 = 0;      // main wings: see enum servoAnimFlutterWings - 0 = both home, 1 = both up, 2 = left home, 3 = left up, 4 = right home, 5 = right up
  a->helper2 = 0;      // alt pos: see enum servoAnimFlutterPos - 0 = none, 1 = home, 2 = up, 3 = down
  a->helper3 = 200;    // flutter offset

  a->function = runServoAnim_flutter;
}


void initServoAnim_range(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_RANGE;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 50;

  a->num_frames = 1;
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
  a->helper1 = 0;      // range: see enum servoAnimRangeSpan - 0 = down to up, 1 = home to up, 2 = alt home to up
  a->helper2 = 0;      // val: 0-9 inclusive
  a->helper3 = 0;

  a->function = runServoAnim_range;
}


void initServoAnim_position(struct ServoAnimation *a) {
  a->id = SERVO_ANIM_POSITION;
  a->active = false;
  a->type = SERVO_ANIM_ALERT;
  a->velocity = 50;

  a->num_frames = 1;
  a->frame_delay = 0;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = 1;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;      // position L: see enum servoAnimPosition - 0 = down, 1 = home, 2 = up
  a->helper2 = 0;      // position R: see enum servoAnimPosition - 0 = down, 1 = home, 2 = up
  a->helper3 = 0;

  a->function = runServoAnim_position;
}


void initServoAnimations() {
    
  onServoAnimDoneCallback = NULL;
  onServoAnimLoopCallback = NULL;

  // all servo animations are initialised when setServoAnim() is called
  initServoAnim_none(&servo_animation_home);
  servo_animation_home.type = SERVO_ANIM_HOME;
  initServoAnim_gentle(&servo_animation_alert);
  servo_animation_alert.type = SERVO_ANIM_ALERT;

  if(RTOS_ENABLED) {

    Mutex_SERVOANIM = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_SERVOANIM_code,     // task function
                      "Task_SERVOANIM",        // name of task
                      STACK_SERVOANIM,         // stack size of task
                      NULL,                    // parameter of the task
                      PRIORITY_SERVOANIM_MID,  // priority of the task (low number = low priority)
                      &Task_SERVOANIM,         // task handle to keep track of created task
                      TASK_CORE_SERVOANIM);    // pin task to core

  }
  
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

int getServoSafeValLeft(int n) {
  int val = n;

  if(n < SERVO_LEFT_DOWN) val = SERVO_LEFT_DOWN;
  if(n > SERVO_LEFT_UP) val = SERVO_LEFT_UP;

  return val;
}

int getServoSafeValRight(int n) {
  int val = n;

  if(n > SERVO_RIGHT_DOWN) val = SERVO_RIGHT_DOWN;
  if(n < SERVO_RIGHT_UP) val = SERVO_RIGHT_UP;

  return val;
}

// ----------------------------------

void initServos(uint8_t mode) {

  if(initialised_servos) return;

  if(mode == SERVO_MODE_INIT_BOTH || mode == SERVO_MODE_INIT_LEFT) {

    wing_left.name = "LEFT";
    wing_left.motor = &s1;
    if(SERVO_LEFT_HOME == -99) {
      Serial << "!! Servo LEFT needs calibration" << endl;
      wing_left.calibrated = false;
    }
    wing_left.home_pos = SERVO_LEFT_HOME;
    wing_left.up_pos = SERVO_LEFT_UP;
    wing_left.down_pos = SERVO_LEFT_DOWN;
    wing_left.current_pos = SERVO_LEFT_UP;

    wing_left.motor->attach(SERVO_LEFT_PIN, SERVO_LEFT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    wing_left.active = true;
    wing_left.motor->setEasingType(EASE_QUADRATIC_IN_OUT);

    wing_left.motor->startEaseTo( SERVO_LEFT_UP, 10 );

  }

  // ---

  if(mode == SERVO_MODE_INIT_BOTH || mode == SERVO_MODE_INIT_RIGHT) {

    wing_right.name = "RIGHT";
    wing_right.motor = &s2;
    if(SERVO_RIGHT_HOME == -99) {
      Serial << "!! Servo RIGHT needs calibration" << endl;
      wing_right.calibrated = false;
    }
    wing_right.home_pos = SERVO_RIGHT_HOME;
    wing_right.up_pos = SERVO_RIGHT_UP;
    wing_right.down_pos = SERVO_RIGHT_DOWN;
    wing_right.current_pos = SERVO_RIGHT_UP;

    wing_right.motor->attach(SERVO_RIGHT_PIN, SERVO_RIGHT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    wing_right.active = true;
    wing_right.motor->setEasingType(EASE_QUADRATIC_IN_OUT);

    wing_right.motor->startEaseTo( SERVO_RIGHT_UP, 10 );
  
  }

  // ---

  if(mode == SERVO_MODE_INIT_BOTH) initialised_servos = true;

}


void Task_SERVOANIM_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_SERVOANIM, (TickType_t)10) == pdTRUE) {
      
      updateServoAnimation();

      if(DEBUG_SERVOANIM_RTOS == true && millis()-last_servoanim_rtos_print >= 1000) {
        Serial << "servoanim stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
        last_servoanim_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_SERVOANIM);
    }
    
    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_SERVOANIM != 0) vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_SERVOANIM );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setServoAnimationTaskPriority(uint8_t p) {
  
  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_SERVOANIM);
  vTaskPrioritySet(Task_SERVOANIM, p);
  if (DEBUG_SERVOANIM_RTOS) Serial << "changed SERVOANIM task priority - new: " << p << " prev: " << prev_priority << endl;

}

