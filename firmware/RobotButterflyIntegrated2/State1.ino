// ---------------------------
// --------- State 1 ---------
// ---------------------------

long last_movement1 = 0;

void setupState1() {

  if(new_enter) {
    new_enter = false;
    Serial << "STATE 1 entrance" << endl;

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
  
  updateNeoAnimation();
  updateServoAnimation();

}

void loopState1() {
  printStateHeartbeat(1);
  if(new_update) {
    new_update = false;
    t_enter = millis(); // eh

    setNeoAnim(&neo_animation_home, NEO_ANIM_ZWOOP, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_OFF);
    setNeoAnimSpeed(&neo_animation_home, 300);
    startNeoAnim(&neo_animation_home);
  }
  
  if(millis()-last_movement1 >= 20000 && t_enter > 30000) {
    
    setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
    setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
    setServoAnimDuration(&servo_animation_alert, 3000);
    startServoAnim(&servo_animation_alert);

    last_movement1 = millis();
  }

  updateServoAnimation();
  updateNeoAnimation();

}
