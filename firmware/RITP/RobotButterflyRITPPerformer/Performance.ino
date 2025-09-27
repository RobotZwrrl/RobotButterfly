void performIndividualAction(uint8_t action) {
  switch(action) {
    case 0: // turn off leds
      neoAnimationOff();
    break;
    case 1: // led animation 1
      neoAnimation1();
    break;
    case 2: // led animation 2
      neoAnimation2();
    break;
  }
}


void performTeamAction(uint8_t action) {
  
  if(action != 0) {
    home_pos_active = false;
    last_pose_movement = millis();
  }
  
  switch(action) {
    case 0: // home position
      homePosAction();
    break;
    case 1: // tilt left
      tiltLeftAction();
    break;
    case 2: // tilt right
      tiltRightAction();
    break;
    case 3: // tilt bwd
      tiltBwdAction();
    break;
    case 4: // tilt fwd
      tiltFwdAction();
    break;
    case 5: // rest action
      restAction();
    break;
    case 6:
      if(!ULTRASONIC_ACTIVE) neoAnimation2();
    break;
  }
}


void neoAnimation1() {
  Serial << "neoAnimation1" << endl;
  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2200);
  startNeoAnim(&neo_animation_alert);

  // setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  // setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME);
  // setServoAnimSpeed(&servo_animation_alert, 100);
  // setServoAnimFlutterOffset(&servo_animation_alert, 100);
  // setServoAnimRepeats(&servo_animation_alert, 3);
  // startServoAnim(&servo_animation_alert);
}


void neoAnimation2() {
  Serial << "neoAnimation2" << endl;

  setNeoAnim(&neo_animation_alert, NEO_ANIM_CYCLE3, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2200);
  startNeoAnim(&neo_animation_alert);

  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // setServoAnimSpeed(&servo_animation_alert, 100);
  // setServoAnimRepeats(&servo_animation_alert, 2);
  // startServoAnim(&servo_animation_alert);
}


void neoAnimationOff() {
  Serial << "neoAnimationOff" << endl;
  setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
  startNeoAnim(&neo_animation_alert);
  setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
  startNeoAnim(&neo_animation_home);

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
  setServoAnimSpeed(&servo_animation_alert, 40);
  setServoAnimFlutterOffset(&servo_animation_alert, 80);
  setServoAnimRepeats(&servo_animation_alert, 2);
  startServoAnim(&servo_animation_alert);
}


void homePosAction() {
  if(home_pos_active == false) {
    Serial << "homePosAction" << endl;
    setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
    setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    setServoAnimDuration(&servo_animation_alert, 500);
    startServoAnim(&servo_animation_alert);
    home_pos_active = true;
  }
}


void tiltLeftAction() {
  Serial << "tiltLeftAction" << endl;
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_LEFT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);
}


void tiltRightAction() {
  Serial << "tiltRightAction" << endl;
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);
}


void tiltBwdAction() {
  Serial << "tiltBwdAction" << endl;
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);
}


void tiltFwdAction() {
  Serial << "tiltFwdAction" << endl;
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_DOWN);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);
}


void restAction() {
  Serial << "rest action" << endl;
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
  setServoAnimDuration(&servo_animation_alert, 3000);
  startServoAnim(&servo_animation_alert);
  last_movement_rest = millis();
}

