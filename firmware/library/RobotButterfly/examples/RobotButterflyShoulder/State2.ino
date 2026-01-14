// ---------------------------
// --------- State 2 ---------
// ---------------------------

long last_proximity_sound = 0;

void setupState2() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE4 << " entrance" << endl;
    new_enter = false;

    // set up our peripheral priorities for this state
    state2Priorities();

    setServoAnim(&servo_animation_home, SERVO_ANIM_NONE, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 500);
    setNeoAnimUno(&neo_animation_alert, 2-1);
    startNeoAnim(&neo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_PURPLE);
    setNeoAnimDuration(&neo_animation_home, 500);
    startNeoAnim(&neo_animation_home);

  }
  
}


void loopState2() {
  if(SERVO_CAL_MODE) return;

  if(new_update) {
    new_update = false;

    stopServoAnim(&servo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_RANGE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GOLDEN_YELLOW, NEO_OFF);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_POSITION, SERVO_ANIM_HOME);
    setServoAnimPositionLeft(&servo_animation_home, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_HOME);
  }

  int proximity8 = getProximity8(&ultrasonic)+1;
  setNeoAnimRangeVal(&neo_animation_home, proximity8);

  //int proximity10 = getProximity10(&ultrasonic)+1;
  if(proximity8 <= 8 && proximity8 > 6) {
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_DOWN);
  } else if(proximity8 <= 6 && proximity8 > 4) {
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_HOME);
  } else if(proximity8 <= 4 && proximity8 >= 0) {
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_UP);
  }

  if(proximity8 <= 2 && millis()-last_proximity_sound >= 6000) {
    if(!MUTE_SOUNDS) playSound(SOUND_FLUTTER_SURPRISE);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_WHITE);
    setNeoAnimSpeed(&neo_animation_alert, 100);
    setNeoAnimDuration(&neo_animation_alert, 2000);
    startNeoAnim(&neo_animation_alert);

    setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
    setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME);
    setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
    setServoAnimSpeed(&servo_animation_alert, 200);
    setServoAnimDuration(&servo_animation_alert, 2000);
    startServoAnim(&servo_animation_alert);

    last_proximity_sound = millis();
  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE2);
}


void state2Priorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(PRIORITY_PROXIMITY_HIGH);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

