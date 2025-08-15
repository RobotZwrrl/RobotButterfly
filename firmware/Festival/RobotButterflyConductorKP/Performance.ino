void restAction() {
  Serial << "rest action" << endl;
  robotbutterfly.conductTeam("0"); // home position
      
  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
  setServoAnimDuration(&servo_animation_alert, 3000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_RED);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_BLUE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_GREEN);
  }
  setNeoAnimDuration(&neo_animation_home, 500);
  startNeoAnim(&neo_animation_home);
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
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);
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
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);
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
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);
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
  if(team == 1) { // red
    setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_WHITE);
  } else if(team == 2) { // blue
    setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  } else { // unknown
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  }
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);
}

