// ---------------------------
// --------- State 3 ---------
// ---------------------------

long last_blip = 0;

void setupState3() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE3 << " entrance" << endl;
    new_enter = false;
    new_update = true;

    state3Priorities(); // set up our peripheral priorities for this state

    setServoAnim(&servo_animation_home, SERVO_ANIM_NONE, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);

    setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 500);
    setNeoAnimUno(&neo_animation_alert, 3-1);
    startNeoAnim(&neo_animation_alert);

  }
}


void loopState3() {

  if(new_update) {
    new_update = false;
    new_enter = true;
    t_enter = millis();

    setServoAnim(&servo_animation_home, SERVO_ANIM_SWAY, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);

    setNeoAnim(&neo_animation_home, NEO_ANIM_RAINBOW, NEO_ANIM_HOME);
    setNeoAnimRainbowSteps(&neo_animation_home, 250);
    startNeoAnim(&neo_animation_home);
  }

  if(millis()-last_blip >= 10000 && t_enter > 15000) {
    
    setServoAnim(&servo_animation_alert, SERVO_ANIM_SWOOSH, SERVO_ANIM_ALERT);
    setServoAnimDuration(&servo_animation_alert, 5000);
    startServoAnim(&servo_animation_alert);
    
    setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_WHITE, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 5000);
    setNeoAnimSpeed(&neo_animation_alert, 300);
    startNeoAnim(&neo_animation_alert);

    last_blip = millis();
  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE1);
}


void state3Priorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

