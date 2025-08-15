void performAction(uint8_t action) {

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
    case 10: // proximity 0
      ; // no sound
    break;
    case 11: { // proximity 1
      if(millis()-last_sound_play >= SONG_PLAY_DELAY) {
        if(CHIRPY_PERFORMER) {
          playSound(SOUND_ALERT_MOONLIGHT_MODE);
        } else {
          playSound(SOUND_SMELL_WILDFLOWER);
        }
        last_sound_play = millis();
      }
    }
    break;
    case 12: { // proximity 2
      if(millis()-last_sound_play >= SONG_PLAY_DELAY) {
        if(CHIRPY_PERFORMER) {
          playSound(SOUND_ALERT_SNEEZE);
        } else {
          playSound(SOUND_SMELL_DAISY);
        }
        last_sound_play = millis();
      }
    }
    break;
    case 13: { // proximity 3
      if(millis()-last_sound_play >= SONG_PLAY_DELAY) {
        if(CHIRPY_PERFORMER) {
          playSound(SOUND_ALERT_MISSION_COMPLETE);
        } else {
          playSound(SOUND_SMELL_SUNFLOWER);
        }
        last_sound_play = millis();
      }
    }
    break;
  }

}


void homePosAction() {
  Serial << "homePosAction" << endl;

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

  // this looks cool and will use it somewhere else
  // setNeoAnim(&neo_animation_alert, NEO_ANIM_ZWOOP, NEO_ANIM_ALERT);
  // if(team == 1) { // red
  //   setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_OFF);
  // } else if(team == 2) { // blue
  //   setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_OFF);
  // } else { // unknown
  //   setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
  // }
  // setNeoAnimSpeed(&neo_animation_alert, 100);
  // setNeoAnimDuration(&neo_animation_alert, 2000);
  // startNeoAnim(&neo_animation_alert);
}


void tiltLeftAction() {
  Serial << "tiltLeftAction" << endl;

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
  Serial << "tiltRightAction" << endl;

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
  Serial << "tiltBwdAction" << endl;

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
  Serial << "tiltFwdAction" << endl;

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

