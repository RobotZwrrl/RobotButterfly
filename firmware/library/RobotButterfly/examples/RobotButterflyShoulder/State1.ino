// ---------------------------
// --------- State 1 ---------
// ---------------------------

long last_movement1 = 0;
long t_enter = 0;
long last_sound1 = 0;
bool sound_choice = false;


void setupState1() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE1 << " entrance" << endl;
    new_enter = false;

    // set up our peripheral priorities for this state
    state1Priorities();

    // servo animation home
    setServoAnim(&servo_animation_home, SERVO_ANIM_POSITION, SERVO_ANIM_HOME);
    setServoAnimPositionLeft(&servo_animation_home, SERVO_ANIM_POSITION_UP);
    setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_HOME);
    startServoAnim(&servo_animation_home);

    // servo animation alert
    setServoAnim(&servo_animation_alert, SERVO_ANIM_NONE, SERVO_ANIM_ALERT);
    startServoAnim(&servo_animation_alert);

    // neo animation home
    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_PURPLE);
    setNeoAnimDuration(&neo_animation_home, 500);
    startNeoAnim(&neo_animation_home);

    // neo animation alert
    setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
    setNeoAnimDuration(&neo_animation_alert, 500);
    setNeoAnimUno(&neo_animation_alert, 1-1);
    startNeoAnim(&neo_animation_alert);
  }
  
}


void loopState1() {
  if(SERVO_CAL_MODE) return;

  if(new_update) {
    new_update = false;
    t_enter = millis(); // eh

    // neo animation home
    setNeoAnim(&neo_animation_home, NEO_ANIM_AMBIANCE, NEO_ANIM_HOME);
    setNeoAnimAmbiance(&neo_animation_home, NEO_ANIM_AMBIANCE_TUTTI_FRUITI);
    startNeoAnim(&neo_animation_home);
  }

  if(millis()-last_movement1 >= 20000 || last_movement1 == 0) {
    
    // play sound
    //if(millis()-last_sound1 >= (60*1000) || last_sound1 == 0) {
      if(sound_choice) {
        if(!MUTE_SOUNDS) playSound(SOUND_FLUTTER_MEMORY);
      } else {
        if(!MUTE_SOUNDS) playSound(SOUND_FLUTTER_CALM);
      }
      sound_choice = !sound_choice;
      last_sound1 = millis();
    //}

    // servo animation alert
    setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
    setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_UP);
    setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
    setServoAnimDuration(&servo_animation_alert, 10000);
    startServoAnim(&servo_animation_alert);

    // neo animation alert
    setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
    setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_PURPLE);
    setNeoAnimDuration(&neo_animation_alert, 10000);
    setNeoAnimSpeed(&neo_animation_alert, 200);
    startNeoAnim(&neo_animation_alert);

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

