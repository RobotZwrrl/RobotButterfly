/*
void OLDupdateButtons() {
  updateButton(&Button_L);
  updateButton(&Button_R);

  // -- double press --
  if(Button_L.state == BUTTON_PRESSED && Button_R.state == BUTTON_PRESSED) {
    if(!double_press) {
      if(DEBUG_BUTTONS) Serial << "both press" << endl;
      double_press = true;
    }
  }

  if(Button_L.state == BUTTON_RELEASED && Button_R.state == BUTTON_RELEASED) {
    if(double_press == true && double_hold == false) {
      if(DEBUG_BUTTONS) Serial << "both press released" << endl;
      double_press = false;
      buttonBothPressCallback();
      return;
    }
  }
  // --

  // -- double long hold --
  if(Button_L.state == BUTTON_LONG_HOLD && Button_R.state == BUTTON_LONG_HOLD) {
    if(!double_hold) {
      if(DEBUG_BUTTONS) Serial << "both long hold" << endl;
      double_hold = true;
    }
  }

  if(Button_L.state == BUTTON_RELEASED && Button_R.state == BUTTON_RELEASED) {
    if(double_hold == true && double_press == false) {
      if(DEBUG_BUTTONS) Serial << "both long hold released" << endl;
      double_hold = false;
      buttonBothLongHoldCallback();
      return;
    }
  }
  // --

}


void OLDupdateButton(volatile struct Button *b) {

  if(b->flag_pressed) {
    if(DEBUG_BUTTONS) Serial << "button " << b->name << " pressed --" << endl;
    b->state = BUTTON_PRESSED;
    b->flag_pressed = false;
    
    if(b->name == 'L') {
      buttonLeftPressCallback();
    } else if(b->name == 'R') {
      buttonRightPressCallback();
    }
  }

  if(b->flag_released) {
    
    if(b->state == BUTTON_LONG_HOLD) {
      b->release_time = millis();
      if(DEBUG_BUTTONS) Serial << "button " << b->name << " released - long hold" << endl;
      if(b->name == 'L') {
        buttonLeftLongHoldCallback();
      } else if(b->name == 'R') {
        buttonRightLongHoldCallback();
      }
    } else {
      b->release_time = millis();
      if(DEBUG_BUTTONS) Serial << "button " << b->name << " released" << endl;
      if(b->name == 'L') {
        buttonLeftReleaseCallback();
      } else if(b->name == 'R') {
        buttonRightReleaseCallback();
      }
    }

    if(b->state != BUTTON_LONG_HOLD && b->release_time-b->press_time <= MAX_CLICK_TIME) {
      if(DEBUG_BUTTONS) Serial << ":: click ::  button " << b->name << " clicked" << endl;
    }
    
    b->state = BUTTON_RELEASED;
    b->flag_released = false;
  }

  if(b->pressed == true && millis()-b->press_time >= LONG_BUTTON_HOLD) {
    if(millis()-b->press_time >= LONG_BUTTON_TIMEOUT) {
      // unblock it incase it's manually locked
      b->pressed = false;
      b->flag_released = true;
      b->release_time = millis();
      //if(DEBUG_BUTTONS) Serial << "button " << b->name << " released (soft)" << endl;
    } else {
      // long button hold
      if(b->state != BUTTON_LONG_HOLD) { // new start of long hold
        if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold start" << endl;
      }
      b->state = BUTTON_LONG_HOLD;
      //if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold - do something" << endl;
    }
  }

}



void initButtons() {
  Button_L.state = BUTTON_RELEASED;
  Button_L.action = BUTTON_IDLE;
  Button_L.pressed = false;
  Button_L.flag_pressed = false;
  Button_L.flag_released = false;
  Button_L.press_time = 0;
  Button_L.release_time = 0;
  Button_L.name = 'L';

  Button_R.state = BUTTON_RELEASED;
  Button_R.action = BUTTON_IDLE;
  Button_R.pressed = false;
  Button_R.flag_pressed = false;
  Button_R.flag_released = false;
  Button_R.press_time = 0;
  Button_R.release_time = 0;
  Button_R.name = 'R';
  
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  attachInterrupt(BUTTON1_PIN, button_R_isr, CHANGE);
  attachInterrupt(BUTTON2_PIN, button_L_isr, CHANGE);
}
*/


