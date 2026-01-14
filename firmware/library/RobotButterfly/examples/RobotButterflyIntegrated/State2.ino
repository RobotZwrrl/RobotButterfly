// ---------------------------
// --------- State 2 ---------
// ---------------------------

void setupState2() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE2 << " entrance" << endl;
    new_enter = false;
    new_update = true;

    state2Priorities(); // set up our peripheral priorities for this state

    setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
    startNeoAnim(&neo_animation_home);

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

  if(new_update) {
    new_update = false;
    new_enter = true;

    setNeoAnim(&neo_animation_home, NEO_ANIM_AMBIANCE, NEO_ANIM_HOME);
    setNeoAnimAmbiance(&neo_animation_home, NEO_ANIM_AMBIANCE_SPRING);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_GENTLE, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);
  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE2);
}


void state2Priorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

