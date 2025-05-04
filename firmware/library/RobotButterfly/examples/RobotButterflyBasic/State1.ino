// ---------------------------
// --------- State 1 ---------
// ---------------------------

void setupState1() {
  if(new_enter) {
    Serial << "STATE " << RobotButterfly::STATE1 << " entrance" << endl;
    new_enter = false;

    // add your one-time init code here for state 1!

    setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
    setServoAnim(&servo_animation_alert, SERVO_ANIM_NONE, SERVO_ANIM_ALERT);

    setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
    setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_CYAN);
    startNeoAnim(&neo_animation_home);

    setServoAnim(&servo_animation_home, SERVO_ANIM_SWAY, SERVO_ANIM_HOME);
    startServoAnim(&servo_animation_home);

  }
  // add your looping init code here for state 1!
}

void loopState1() {
  if(new_update) {
    // add your one-time behaviour code here for state 1!
    new_update = false;
  }
  // add your looping behaviour code here for state 1!

  if(SERVO_CAL_MODE) {

    if(button_calib_changed) {

      if(left_cal_mode == 0) { // wing up
        servo_calib_pos_left = SERVO_ANIM_POSITION_UP;
      } else if(left_cal_mode == 1) { // wing home
        servo_calib_pos_left = SERVO_ANIM_POSITION_HOME;
      } else if(left_cal_mode == 2) { // wing down
        servo_calib_pos_left = SERVO_ANIM_POSITION_DOWN;
      }

      if(right_cal_mode == 0) { // wing up
        servo_calib_pos_right = SERVO_ANIM_POSITION_UP;
      } else if(right_cal_mode == 1) { // wing home
        servo_calib_pos_right = SERVO_ANIM_POSITION_HOME;
      } else if(right_cal_mode == 2) { // wing down
        servo_calib_pos_right = SERVO_ANIM_POSITION_DOWN;
      }

      setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
      setServoAnimRepeats(&servo_animation_alert, -99);
      setServoAnimPositionLeft(&servo_animation_alert, servo_calib_pos_left);
      setServoAnimPositionRight(&servo_animation_alert, servo_calib_pos_right);
      startServoAnim(&servo_animation_alert);

      setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
      setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_GREEN);
      setNeoAnimSpeed(&neo_animation_alert, 1000);
      setNeoAnimUno(&neo_animation_alert, left_cal_mode); // top-most leds near the back
      setNeoAnimDuo(&neo_animation_alert, 5+right_cal_mode); // bottom-most leds near the front
      startNeoAnim(&neo_animation_alert);

      button_calib_changed = false;
    }

  }

  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE1);
}

