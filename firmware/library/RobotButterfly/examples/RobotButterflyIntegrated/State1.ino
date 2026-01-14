// ---------------------------
// --------- State 1 ---------
// ---------------------------

long last_movement1 = 0;
long t_enter = 0;

void setupState1() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE1 << " entrance" << endl;
    new_enter = false;
    new_update = true;

    state1Priorities(); // set up our peripheral priorities for this state

    setServoAnim(&servo_animation_home, SERVO_ANIM_POSITION, SERVO_ANIM_HOME);
    setServoAnimPositionLeft(&servo_animation_home, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_UP);
    startServoAnim(&servo_animation_home);

    setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
    setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
    startServoAnim(&servo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
    startNeoAnim(&neo_animation_home);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 500);
    setNeoAnimUno(&neo_animation_alert, 1-1);
    startNeoAnim(&neo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_PURPLE);
    setNeoAnimDuration(&neo_animation_home, 500);
    startNeoAnim(&neo_animation_home);

  }
}


void loopState1() {

  if(new_update) {
    new_update = false;
    new_enter = true;
    t_enter = millis();

    setNeoAnim(&neo_animation_home, NEO_ANIM_ZWOOP, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_OFF);
    setNeoAnimSpeed(&neo_animation_home, 300);
    startNeoAnim(&neo_animation_home);
  }

  if(millis()-last_movement1 >= 20000 && millis()-t_enter > 30000) {
    
    setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
    setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
    setServoAnimDuration(&servo_animation_alert, 3000);
    startServoAnim(&servo_animation_alert);

    last_movement1 = millis();
  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE1);
}


void state1Priorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

