// ---------------------------
// --------- State 1 ---------
// ---------------------------

void setupState1() {

  if(new_enter) {
    new_enter = false;
    Serial << "STATE 1 entrance" << endl;

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
  
  updateNeoAnimation();

}

void loopState1() {
  
  printStateHeartbeat(1);

  if(new_update) {
    new_update = false;

    setNeoAnim(&neo_animation_home, NEO_ANIM_AMBIANCE, NEO_ANIM_HOME);
    setNeoAnimAmbiance(&neo_animation_home, NEO_ANIM_AMBIANCE_SPRING);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_GENTLE, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);
  }

  // less laggy with some commented out
  updateServoAnimation();
  //updateSensors();
  //updateProximity();
  updateNeoAnimation();
  //updateIMU();
  
}

