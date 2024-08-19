// TODO: Semaphores for tasks to react to buttons events

void updateButtons() {
  updateButton(&Button_L);
  updateButton(&Button_R);
}



void updateButton(volatile struct Button *b) {

  if(!b->printed_pressed) {
    if(DEBUG_BUTTONS) Serial << "button " << b->name << " pressed --" << endl;
    b->state = BUTTON_PRESSED;
    b->printed_pressed = true;
  }

  if(!b->printed_released) {
    if(DEBUG_BUTTONS) Serial << "button " << b->name << " released" << endl;
    b->state = BUTTON_RELEASED;
    b->printed_released = true;
  }

  if(b->pressed == true && millis()-b->press_time >= LONG_BUTTON_HOLD) {
    if(millis()-b->press_time >= LONG_BUTTON_TIMEOUT) {
      // unblock it incase it's manually locked
      b->pressed = false;
      b->state = BUTTON_RELEASED;
      b->printed_released = false;
      if(DEBUG_BUTTONS) Serial << "button " << b->name << " released (soft)" << endl;
    } else {
      // long button hold
      if(b->state != BUTTON_LONG_HOLD) { // new start of long hold
        if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold flag" << endl;
      }
      b->state = BUTTON_LONG_HOLD;
      if(DEBUG_BUTTONS) Serial << "button " << b->name << " long hold - do something" << endl;
    }
  }

}



void initButtons() {
  Button_L.state = BUTTON_RELEASED;
  Button_L.pressed = false;
  Button_L.printed_pressed = false;
  Button_L.printed_released = false;
  Button_L.press_time = 0;
  Button_L.release_time = 0;
  Button_L.name = 'L';

  Button_R.state = BUTTON_RELEASED;
  Button_R.pressed = false;
  Button_R.printed_pressed = false;
  Button_R.printed_released = false;
  Button_R.press_time = 0;
  Button_R.release_time = 0;
  Button_R.name = 'R';
  
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  attachInterrupt(BUTTON1_PIN, button_R_isr, CHANGE);
  attachInterrupt(BUTTON2_PIN, button_L_isr, CHANGE);
}



