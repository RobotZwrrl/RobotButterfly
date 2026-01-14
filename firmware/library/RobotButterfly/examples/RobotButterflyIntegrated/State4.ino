// ---------------------------
// --------- State 4 ---------
// ---------------------------

long last_proximity_sound = 0;

void setupState4() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE4 << " entrance" << endl;
    new_enter = false;
    new_update = true;

    state4Priorities(); // set up our peripheral priorities for this state

    setServoAnim(&servo_animation_home, SERVO_ANIM_NONE, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 500);
    setNeoAnimUno(&neo_animation_alert, 4-1);
    startNeoAnim(&neo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_PURPLE);
    setNeoAnimDuration(&neo_animation_home, 500);
    startNeoAnim(&neo_animation_home);

  }
}


void loopState4() {

  if(new_update) {
    new_update = false;
    new_enter = true;

    stopServoAnim(&servo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_RANGE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GOLDEN_YELLOW, NEO_OFF);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_RANGE, SERVO_ANIM_HOME);
    setServoAnimRangeSpan(&servo_animation_home, SERVO_ANIM_RANGE_DOWN_UP);
    startServoAnim(&servo_animation_home);
  }

  int proximity8 = getProximity8(&ultrasonic, true)+1; // boolean parameter = if sensor data is raw or averaged val
  setNeoAnimRangeVal(&neo_animation_home, proximity8);

  int proximity10 = getProximity10(&ultrasonic, true)+1; // boolean parameter = if sensor data is raw or averaged val
  setServoAnimRangeVal(&servo_animation_home, proximity10);

  if(proximity8 <= 2 && millis()-last_proximity_sound >= 6000) {
    playSound(SOUND_FLUTTER_SURPRISE);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_WHITE);
    setNeoAnimSpeed(&neo_animation_alert, 100);
    setNeoAnimDuration(&neo_animation_alert, 2000);
    startNeoAnim(&neo_animation_alert);

    setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
    setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME);
    setServoAnimSpeed(&servo_animation_alert, 200);
    setServoAnimDuration(&servo_animation_alert, 2000);
    startServoAnim(&servo_animation_alert);

    last_proximity_sound = millis();
  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE4);
}


void state4Priorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(PRIORITY_PROXIMITY_HIGH);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

