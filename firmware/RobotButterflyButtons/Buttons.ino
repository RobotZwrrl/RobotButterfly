
void buttonDoublePress() {
  if(DEBUG_BUTTONS) Serial << "both press" << endl;
}

void buttonDoublePressRelease() {
  if(DEBUG_BUTTONS) Serial << "both press released" << endl;
}

void buttonDoubleLongHoldRelease() {
  if(DEBUG_BUTTONS) Serial << "both long hold released" << endl;
}


// this loop uses the states and flags set to prioritise the
// order of operations for calling the callbacks
void updateButtons() {
  updateButton(&Button_L);
  updateButton(&Button_R);

  // both buttons pressed
  if(Button_L.state == BUTTON_PRESSED && Button_R.state == BUTTON_PRESSED) {
    if(!double_press) {
      double_press = true;
      buttonDoublePress();
    }
  } else {

    if(double_press == false && (millis()-double_press_release_time >= 100)) { // flag must be cleared
      //Serial << "yes" << endl;
      // one or the other pressed
      // but only callback after a certain amount of time 
      // to ensure that both are not pressed
      if(Button_L.state == BUTTON_PRESSED && Button_R.state != BUTTON_PRESSED && (millis()-Button_L.press_time) >= DOUBLE_CALLBACK_TIMEOUT) {
        if(!Button_L.flag_press_notif) {
          buttonPress(&Button_L);
          Button_L.flag_press_notif = true;
        } else {
          buttonHold(&Button_L);
        }
      }
      if(Button_L.state != BUTTON_PRESSED && Button_R.state == BUTTON_PRESSED && (millis()-Button_R.press_time) >= DOUBLE_CALLBACK_TIMEOUT) {
        if(!Button_R.flag_press_notif) {
          buttonPress(&Button_R);
          Button_R.flag_press_notif = true;
        } else {
          buttonHold(&Button_R);
        }
      }
    }
  }

  // both buttons released
  if(Button_L.state == BUTTON_RELEASED && Button_R.state == BUTTON_RELEASED) {
    // option A: double press
    if(double_press == true && double_hold == false) {
      double_press = false;
      double_press_release_time = millis();
      Button_L.state = BUTTON_IDLE;
      Button_R.state = BUTTON_IDLE;
      buttonDoublePressRelease();
    }
    // option B: double hold
    if(double_hold == true && double_press == false) {
      double_hold = false;
      double_hold_release_time = millis();
      Button_L.state = BUTTON_IDLE;
      Button_R.state = BUTTON_IDLE;
      buttonDoubleLongHoldRelease();
    }
  } else {
    // one or the other released
    // but only callback after a certain amount of time 
    // to ensure that both are not pressed

    if(double_press == false && (millis()-double_press_release_time >= 100)) { // flag must be cleared

      if(Button_L.state == BUTTON_RELEASED && Button_R.state != BUTTON_RELEASED && (millis()-Button_L.release_time) >= DOUBLE_CALLBACK_TIMEOUT) {
        Button_L.state = BUTTON_IDLE;
        if(Button_L.release_time-Button_L.press_time <= MAX_CLICK_TIME) {
          buttonClick(&Button_L);
        } else {
          buttonRelease(&Button_L);
        }
        Button_L.flag_press_notif = false;
      }
      if(Button_L.state != BUTTON_RELEASED && Button_R.state == BUTTON_RELEASED && (millis()-Button_L.release_time) >= DOUBLE_CALLBACK_TIMEOUT) {
        Button_R.state = BUTTON_IDLE;
        if(Button_R.release_time-Button_R.press_time <= MAX_CLICK_TIME) {
          buttonClick(&Button_R);
        } else {
          buttonRelease(&Button_R);
        }
        Button_R.flag_press_notif = false;
      }
    }

  }
  

  
  // both buttons long hold
  if(Button_L.state == BUTTON_LONG_HOLD && Button_R.state == BUTTON_LONG_HOLD) {
    if(!double_hold) {
      double_hold = true;
      if(Button_L.flag_notif == true || Button_R.flag_notif == true) {
        buttonDoubleLongHoldNotif();
        Button_L.flag_notif = false;
        Button_R.flag_notif = false;
      } else {
        buttonDoubleLongHold();
      }
    }
  } else {

    if(!double_hold) {
      // one or the other long hold
      
      if(Button_L.state == BUTTON_LONG_HOLD && Button_R.state != BUTTON_LONG_HOLD) {
        if(Button_L.flag_notif) {
          buttonLongHoldNotif(&Button_L);
          Button_L.flag_notif = false;
        } else {
          buttonLongHold(&Button_L);
        }
      }
      if(Button_L.state != BUTTON_LONG_HOLD && Button_R.state == BUTTON_LONG_HOLD) {
        if(Button_R.flag_notif) {
          buttonLongHoldNotif(&Button_R);
          Button_R.flag_notif = false;
        } else {
          buttonLongHold(&Button_R);
        }
      }

    }
  }


}


// callbacks are now called from the main button update
// loop, but are preserved here via comments just in case.
// this function is more about setting the individual flags
// and states. the main button loop prioritises & sends callbacks.
void updateButton(volatile struct Button *b) {

  if(b->flag_pressed) {
    b->state = BUTTON_PRESSED;
    b->flag_pressed = false;
    //buttonPress(b);
  }

  if(b->flag_released) {
    
    b->release_time = millis();

    // if(b->state == BUTTON_LONG_HOLD) {
    //   buttonLongHoldRelease(b);
    // } else {
    //   if(b->state != BUTTON_LONG_HOLD && b->release_time-b->press_time <= MAX_CLICK_TIME) {
    //     buttonClick(b);
    //   }
    //   buttonRelease(b);
    // }
    
    b->state = BUTTON_RELEASED; // state changes to IDLE in update loop
    b->flag_released = false;
    b->flag_notif = false;
  }

  if(b->pressed == true && millis()-b->press_time >= LONG_BUTTON_HOLD) {
    if(millis()-b->press_time >= LONG_BUTTON_TIMEOUT) {
      // send a notification to the user that long hold is activated
      b->state = BUTTON_LONG_HOLD;
      //if(!b->flag_notif) {
        //buttonHoldNotif(b);
      //}
      b->flag_notif = true;
    } else {
      // long button hold
      //if(b->state != BUTTON_LONG_HOLD) { // new start of long hold
        //buttonLongHoldStart(b);
      //}
      b->state = BUTTON_LONG_HOLD;
    }
  }

}




void buttonDoubleLongHoldNotif() {
  if(DEBUG_BUTTONS) Serial << "both button long hold notification" << endl;
}

void buttonDoubleLongHold() {
  if(DEBUG_BUTTONS) Serial << "both button long hold" << endl;
}


void buttonLongHoldRelease(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold release" << endl;
}

void buttonLongHold(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold" << endl;
}

void buttonClick(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " clicked (aka, released within MAX_CLICK_TIME)" << endl;
  // TODO
}

void buttonRelease(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " released" << endl;
  // if(b->name == 'L') {
  //   buttonLeftPressCallback();
  // } else if(b->name == 'R') {
  //   buttonRightPressCallback();
  // }

}

void buttonLongHoldNotif(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold notification" << endl;
  // if(Button_L.state == BUTTON_LONG_HOLD && Button_R.state == BUTTON_LONG_HOLD) {
  //   buttonBothHoldNotifCallback();
  // } else if(Button_L.state == BUTTON_LONG_HOLD && Button_R.state != BUTTON_LONG_HOLD) {
  //   buttonLeftHoldNotifCallback();
  // } else if(Button_L.state != BUTTON_LONG_HOLD && Button_R.state == BUTTON_LONG_HOLD) {
  //   buttonRightHoldNotifCallback();
  // }
}

void buttonPress(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " pressed" << endl;
  // if(b->name == 'L') {
  //   buttonLeftPressCallback();
  // } else if(b->name == 'R') {
  //   buttonRightPressCallback();
  // }
}

void buttonHold(volatile struct Button *b) {
  if(DEBUG_BUTTONS) Serial << "button " << b->name << " hold2" << endl;
  // if(b->name == 'L') {
  //   buttonLeftPressCallback();
  // } else if(b->name == 'R') {
  //   buttonRightPressCallback();
  // }
}


void initButtons() {
  Button_L.state = BUTTON_RELEASED;
  Button_L.pressed = false;
  Button_L.flag_pressed = false;
  Button_L.flag_released = false;
  Button_L.flag_notif = false;
  Button_L.flag_press_notif = false;
  Button_L.press_time = 0;
  Button_L.release_time = 0;
  Button_L.name = 'L';

  Button_R.state = BUTTON_RELEASED;
  Button_R.pressed = false;
  Button_R.flag_pressed = false;
  Button_R.flag_released = false;
  Button_R.flag_notif = false;
  Button_R.flag_press_notif = false;
  Button_R.press_time = 0;
  Button_R.release_time = 0;
  Button_R.name = 'R';
  
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  attachInterrupt(BUTTON1_PIN, button_R_isr, CHANGE);
  attachInterrupt(BUTTON2_PIN, button_L_isr, CHANGE);
}



