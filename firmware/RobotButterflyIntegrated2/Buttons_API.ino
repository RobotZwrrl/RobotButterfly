// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------

// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      playSimpleTone(NOTE_F5, 500);
      playNoTone();
      delay(200);
    break;
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 500);
      playNoTone();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 500);
      playNoTone();
    break;
  }
}

// do an action here
void buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH: {
      counter *= 2;

      // if it's within the first 30 seconds
      if(millis() < 30*1000 && SERVO_CAL_MODE == false) {
        SERVO_CAL_MODE = true;
        button_calib_changed = true;
        return;
      }

      if(SERVO_CAL_MODE == true) {
        SERVO_CAL_MODE = false;
        // re-enter the state
        changeState(CURRENT_STATE);
        return;
      }
    }
    break;
    case BUTTON_LEFT:
      counter += 10;
    break;
    case BUTTON_RIGHT:
      counter -= 10;
    break;
  }
  Serial << "count: " << counter << endl;
}

// do an action here
void buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      if(!SERVO_CAL_MODE) {
        incrementState();
        return;
      } else {

        if(left_cal_dir) {
          left_cal_mode++;
        } else {
          left_cal_mode--;
        }
        
        if(left_cal_mode >= 2) left_cal_dir = !left_cal_dir;
        if(left_cal_mode <= 0) left_cal_dir = !left_cal_dir;
        button_calib_changed = true;
      }
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      if(!SERVO_CAL_MODE) {
        decrementState();
        return;
      } else {

        if(right_cal_dir) {
          right_cal_mode++;
        } else {
          right_cal_mode--;
        }
        
        if(right_cal_mode >= 2) right_cal_dir = !right_cal_dir;
        if(right_cal_mode <= 0) right_cal_dir = !right_cal_dir;
        button_calib_changed = true;
      }
    break;
  }
}

// probably not necessary to do anything here
void buttonReleaseCallback(uint8_t n) {

}

