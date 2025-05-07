// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------

void buttonHoldNotificationCallback(uint8_t n) {
  
  if(n == BUTTON_BOTH) {

    if(!hold_notif_action) {

      Serial << "see!" << endl;

      setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
      setServoAnim(&servo_animation_home, SERVO_ANIM_NONE, SERVO_ANIM_HOME);

      setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
      setServoAnimRepeats(&servo_animation_alert, -99);
      setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
      setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
      startServoAnim(&servo_animation_alert);

      setNeoAnim(&neo_animation_alert, NEO_ANIM_ZWOOP, NEO_ANIM_ALERT);
      
      if(SERVO_CAL_MODE) {
        setNeoAnimColours(&neo_animation_alert, NEO_PURPLE, NEO_OFF);
      } else {
        setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_OFF);
      }
      
      setNeoAnimSpeed(&neo_animation_alert, 500);
      startNeoAnim(&neo_animation_alert);
      hold_notif_action = true;
    }

  }
}

void buttonHoldReleasedCallback(uint8_t n) {
  
  if(n == BUTTON_BOTH) {

    if(SERVO_CAL_MODE == true) {
      Serial << "exiting servo cal mode" << endl;
      SERVO_CAL_MODE = false;
      robotbutterfly.changeState(RobotButterfly::CURRENT_STATE); // re-enter the state
    } else {
      if(millis() < 30*1000) {
        Serial << "entering servo cal mode" << endl;
        button_calib_changed = true;
        SERVO_CAL_MODE = true;
      }
    }
    hold_notif_action = false;
  }

}

void buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT: {
      
      if(!SERVO_CAL_MODE) {
        robotbutterfly.incrementState();
        return;
      }

      if(left_cal_dir) {
        left_cal_mode++;
      } else {
        left_cal_mode--;
      }
      
      if(left_cal_mode >= 2) left_cal_dir = !left_cal_dir;
      if(left_cal_mode <= 0) left_cal_dir = !left_cal_dir;
      button_calib_changed = true;

      Serial << "left: " << left_cal_mode << endl;

    }
    break;
    case BUTTON_RIGHT: {

      if(!SERVO_CAL_MODE) {
        robotbutterfly.decrementState();
        return;
      }

      if(right_cal_dir) {
        right_cal_mode++;
      } else {
        right_cal_mode--;
      }
      
      if(right_cal_mode >= 2) right_cal_dir = !right_cal_dir;
      if(right_cal_mode <= 0) right_cal_dir = !right_cal_dir;
      button_calib_changed = true;

      Serial << "right: " << right_cal_mode << endl;

    }
    break;
  }
}


