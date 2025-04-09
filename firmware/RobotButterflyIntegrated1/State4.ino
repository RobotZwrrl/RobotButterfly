// ---------------------------
// --------- State 4 ---------
// ---------------------------

long last_proximity_sound = 0;

void setupState4() {
  if(new_enter) {
    new_enter = false;
    Serial << "STATE 4 entrance" << endl;

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

  updateNeoAnimation();
  
}


void loopState4() {
  printStateHeartbeat(4);
  
  if(new_update) {
    new_update = false;

    stopServoAnim(&servo_animation_alert);

    setNeoAnim(&neo_animation_home, NEO_ANIM_RANGE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GOLDEN_YELLOW, NEO_OFF);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_RANGE, SERVO_ANIM_HOME);
    setServoAnimRangeSpan(&servo_animation_home, SERVO_ANIM_RANGE_DOWN_UP);
    startServoAnim(&servo_animation_home);
  }

  // less laggy with some commented out
  updateServoAnimation();
  //updateSensors();
  updateProximity();
  updateNeoAnimation();
  //updateIMU();

  int proximity8 = getProximity8(&ultrasonic)+1;
  setNeoAnimRangeVal(&neo_animation_home, proximity8);

  int proximity10 = getProximity10(&ultrasonic)+1;
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

}

