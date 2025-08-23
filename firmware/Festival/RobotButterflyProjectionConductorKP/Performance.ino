void restAction() {

  Serial << "rest action" << endl;
  
  robotbutterfly.conductTeam("5"); // rest action
      
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
  setServoAnimDuration(&servo_animation_alert, 3000);
  startServoAnim(&servo_animation_alert);

  last_movement_rest = millis();
}


void tiltLeftAction() {

  Serial << "tilt left!" << endl;

  robotbutterfly.conductTeam("1"); // tilt left

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_LEFT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_WILDFLOWER);
}


void tiltRightAction() {

  Serial << "tilt right!" << endl;

  robotbutterfly.conductTeam("2"); // tilt right

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_DAISY);
}


void tiltBwdAction() {

  Serial << "tilt bwd!" << endl;

  robotbutterfly.conductTeam("3"); // tilt bwd

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_SUNFLOWER);
}


void tiltFwdAction() {
  
  Serial << "tilt fwd!" << endl;

  robotbutterfly.conductTeam("4"); // tilt fwd

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_DOWN);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
  if(team == RED_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == BLUE_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else if(team == OTHER_TEAM) {
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_LAVENDER);
}


void homePosAction() {
  if(millis()-last_movement_rest <= 3000) return;

  if(home_pos_active == false) {

    Serial << "home pos!" << endl;

    robotbutterfly.conductTeam("0"); // home pos

    setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
    setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    setServoAnimSpeed(&servo_animation_alert, 100);
    startServoAnim(&servo_animation_alert);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
    setNeoAnimDuration(&neo_animation_alert, 100);
    startNeoAnim(&neo_animation_alert);

    home_pos_active = true;
  }
}

