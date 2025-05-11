#include "modules/Buttons/Buttons.h"

ButtonCallback onHoldNotificationCallback = NULL;
ButtonCallback onHoldReleasedCallback = NULL;
ButtonCallback onClickCallback = NULL;
ButtonCallback onReleaseCallback = NULL;

void buttonRChanged();
void buttonLChanged();
void updateButton(struct Button *b);
void buttonClicked(struct Button *b);
void buttonReleasedBoth();
void buttonHoldBoth();
void buttonReleased(struct Button *b);
void buttonHold(struct Button *b);

// -----------------------------------
// -----------------------------------



// ----------- buttons isr -----------
void IRAM_ATTR button_L_isr() {
  button_L_changed = true;
}

void IRAM_ATTR button_R_isr() {
  button_R_changed = true;
}
// -----------------------------------


void buttonHold(struct Button *b) {

  if(Button_L.state == BUTTON_BOTH_HOLD || Button_R.state == BUTTON_BOTH_HOLD) return;

  if(b->flag_state_change) {
    if(DEBUG_BUTTONS) Serial << "!! ";
    if(b->name == 'L') {
      if(onHoldNotificationCallback) onHoldNotificationCallback(BUTTON_LEFT);
    }
    if(b->name == 'R') {
      if(onHoldNotificationCallback) onHoldNotificationCallback(BUTTON_RIGHT);
    }
  }
  if(b->flag_button_hold_notif) {
    if(DEBUG_BUTTONS) Serial << "time !! ";
  }
  if(DEBUG_BUTTONS) Serial << "buttonHold " << b->name << endl;
}

void buttonReleased(struct Button *b) {
  if(b->flag_state_change) {
    if(DEBUG_BUTTONS) Serial << "!! ";
    if(DEBUG_BUTTONS) Serial << "buttonReleased " << b->name << endl;

    if(b->state_prev == BUTTON_HOLD) {
      if(b->name == 'L') {
        if(onHoldReleasedCallback) onHoldReleasedCallback(BUTTON_LEFT);
      }
      if(b->name == 'R') {
        if(onHoldReleasedCallback) onHoldReleasedCallback(BUTTON_RIGHT);
      }
    } else {
      if(b->name == 'L') {
        if(onReleaseCallback) onReleaseCallback(BUTTON_LEFT);
      }
      if(b->name == 'R') {
        if(onReleaseCallback) onReleaseCallback(BUTTON_RIGHT);
      }
    }
  }
  b->flag_button_hold_notif = false;
}

void buttonHoldBoth() {
  if(Button_L.flag_state_change == true || Button_R.flag_state_change == true) {
    if(DEBUG_BUTTONS) Serial << "!! ";
  }
  if(Button_L.flag_button_hold_notif == true || Button_R.flag_button_hold_notif == true) {
    if(DEBUG_BUTTONS) Serial << "time !! ";
    if(Button_L.flag_state_change == true || Button_R.flag_state_change == true) {
      if(onHoldNotificationCallback) onHoldNotificationCallback(BUTTON_BOTH);
    }
  }
  if(DEBUG_BUTTONS) Serial << "buttonHoldBoth" << endl;
}

void buttonReleasedBoth() {
  if(Button_L.flag_state_change == true || Button_R.flag_state_change == true) {
    if(DEBUG_BUTTONS) Serial << "!! ";
    if(onHoldReleasedCallback) onHoldReleasedCallback(BUTTON_BOTH);
  }
  if(DEBUG_BUTTONS) Serial << "buttonReleasedBoth" << endl;
}

void buttonClicked(struct Button *b) {
  if(b->flag_state_change) {
    if(DEBUG_BUTTONS) Serial << "!! ";
  }

  if(b->name == 'L') {
    if(onClickCallback) onClickCallback(BUTTON_LEFT);
  }
  if(b->name == 'R') {
    if(onClickCallback) onClickCallback(BUTTON_RIGHT);
  }

  if(DEBUG_BUTTONS) Serial << "buttonClicked " << b->name << endl;
  b->flag_button_hold_notif = false;
}



void updateButtons() {
  updateButton(&Button_L);
  updateButton(&Button_R);

  // ------------ hold ------------
  if(Button_L.state == BUTTON_HOLD && Button_R.state == BUTTON_HOLD) {
    Button_L.state = Button_L.state_prev;
    Button_L.state = BUTTON_BOTH_HOLD;
    Button_R.state = Button_R.state_prev;
    Button_R.state = BUTTON_BOTH_HOLD;
    buttonHoldBoth();
    return;
  } else if(Button_L.state == BUTTON_BOTH_HOLD || Button_R.state == BUTTON_BOTH_HOLD) {
    buttonHoldBoth();
    return;
  } else {
    if(Button_L.state == BUTTON_HOLD && millis()-Button_L.press_time >= NEXT_INDIVIDUAL_BUTTON_HOLD) {
      if(Button_L.state_prev != BUTTON_BOTH_HOLD || Button_R.state_prev != BUTTON_BOTH_HOLD) {
        if(millis()-Button_L.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT && millis()-Button_R.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT) {
          buttonHold(&Button_L);
        }
        return;
      }
    }
    if(Button_R.state == BUTTON_HOLD && millis()-Button_R.press_time >= NEXT_INDIVIDUAL_BUTTON_HOLD) {
      if(Button_L.state_prev != BUTTON_BOTH_HOLD || Button_R.state_prev != BUTTON_BOTH_HOLD) {
        if(millis()-Button_L.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT && millis()-Button_R.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT) {
          buttonHold(&Button_R);
        }
        return;
      }
    }
  }
  // ------------------------------

  // ---------- clicked ----------
  if(Button_L.state == BUTTON_CLICK || Button_R.state == BUTTON_CLICK) {
    if(Button_L.state == BUTTON_CLICK) buttonClicked(&Button_L);
    if(Button_R.state == BUTTON_CLICK) buttonClicked(&Button_R);
    return;
  }
  // ------------------------------

  // ---------- released ----------
  if(Button_L.state == BUTTON_RELEASED || Button_R.state == BUTTON_RELEASED) {
    
    if(Button_L.state_prev == BUTTON_BOTH_HOLD || Button_R.state_prev == BUTTON_BOTH_HOLD) {
      Button_L.release_both_time = millis();
      Button_R.release_both_time = millis();
      buttonReleasedBoth();
      return;
    } else {
      if(millis()-Button_L.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT && millis()-Button_R.release_both_time >= BUTTON_BOTH_RELEASE_LOCKOUT) {
        if(Button_L.state == BUTTON_RELEASED) buttonReleased(&Button_L);
        if(Button_R.state == BUTTON_RELEASED) buttonReleased(&Button_R);
        return;
      }
    }
  }
  // ------------------------------

}



void updateButton(struct Button *b) {

  b->state_prev = b->state;

  if(b->flag_pressed) {
    b->state = BUTTON_PRESSED;
    b->flag_pressed = false;
  }

  if(b->state == BUTTON_CLICK && b->flag_released == false) {
    b->state = BUTTON_IDLE;
  }

  if(b->flag_released) {
    
    b->release_time = millis();
    b->state = BUTTON_RELEASED;

    if(b->release_time-b->press_time <= MAX_CLICK_TIME) {
      b->state = BUTTON_CLICK;
      b->click_time = millis();
    }

    b->flag_released = false;
  }

  if(b->pressed == true && millis()-b->press_time >= BUTTON_HOLD_DURATION) {
    if(millis()-b->press_time >= BUTTON_HOLD_NOTIF_DURATION && b->flag_button_hold_notif == false) {
      // send a notification to the user that long hold is activated
      b->flag_button_hold_notif = true;
    }
    // long button hold
    b->state = BUTTON_HOLD;
  }

  if(b->state_prev != b->state) {
    b->flag_state_change = true;
  } else {
    b->flag_state_change = false;
  }

  buttonLChanged();
  buttonRChanged();

}


void buttonLChanged() {
  if(button_L_changed) {
    struct Button *b = &Button_L;
  
    if(digitalRead(BUTTON2_PIN) == HIGH) { // pressed
      if(millis()-b->press_time <= DEBOUNCE_TIME && b->press_time > 0) { // debounce time
        return;
      }
      if(millis()-b->release_time <= ACCIDENTAL_CLICK_TIME && b->release_time > 0) { // accidental double click
        return; 
      }
      b->pressed = true;
      b->flag_pressed = true;
      b->press_time = millis();
    } else { // released
      if(millis()-b->release_time <= DEBOUNCE_TIME && b->release_time > 0) { // debounce time
        return;
      }
      b->flag_released = true;
      b->pressed = false;
      b->release_time = millis();
    }
    button_L_changed = false;
  }
}


void buttonRChanged() {
  if(button_R_changed) {
    struct Button *b = &Button_R;
  
    if(digitalRead(BUTTON1_PIN) == HIGH) { // pressed
      if(millis()-b->press_time <= DEBOUNCE_TIME && b->press_time > 0) { // debounce time
        return;
      }
      if(millis()-b->release_time <= ACCIDENTAL_CLICK_TIME && b->release_time > 0) { // accidental double click
        return; 
      }
      b->pressed = true;
      b->flag_pressed = true;
      b->press_time = millis();
    } else { // released
      if(millis()-b->release_time <= DEBOUNCE_TIME && b->release_time > 0) { // debounce time
        return;
      }
      b->flag_released = true;
      b->pressed = false;
      b->release_time = millis();
    }
    button_R_changed = false;
  }
}


void initButtons() {
  ButtonCallback onHoldNotificationCallback = NULL;
  ButtonCallback onHoldReleasedCallback = NULL;
  ButtonCallback onClickCallback = NULL;
  ButtonCallback onReleaseCallback = NULL;

  button_L_changed = false;
  button_R_changed = false;

  Button_L.state = BUTTON_IDLE;
  Button_L.state_prev = BUTTON_IDLE;
  Button_L.pressed = false;
  Button_L.flag_pressed = false;
  Button_L.flag_released = false;
  Button_L.flag_state_change = false;
  Button_L.flag_button_hold_notif = false;
  Button_L.press_time = 0;
  Button_L.release_time = 0;
  Button_L.release_both_time = 0;
  Button_L.click_time = 0;
  Button_L.name = 'L';

  Button_R.state = BUTTON_IDLE;
  Button_R.state_prev = BUTTON_IDLE;
  Button_R.pressed = false;
  Button_R.flag_pressed = false;
  Button_R.flag_released = false;
  Button_R.flag_state_change = false;
  Button_R.flag_button_hold_notif = false;
  Button_R.press_time = 0;
  Button_R.release_time = 0;
  Button_R.release_both_time = 0;
  Button_R.click_time = 0;
  Button_R.name = 'R';
  
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  attachInterrupt(BUTTON1_PIN, button_R_isr, CHANGE);
  attachInterrupt(BUTTON2_PIN, button_L_isr, CHANGE);

  if(RTOS_ENABLED) {

    Mutex_BUTTONS = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_BUTTONS_code,     // task function
                      "Task_BUTTONS",        // name of task
                      STACK_BUTTONS,         // stack size of task
                      NULL,                  // parameter of the task
                      PRIORITY_BUTTONS_MID,  // priority of the task
                      &Task_BUTTONS,         // task handle to keep track of task
                      TASK_CORE_BUTTONS);    // pin task to core

  }

}


void Task_BUTTONS_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_BUTTONS, (TickType_t)10) == pdTRUE) {
      
      updateButtons();

      if(DEBUG_BUTTONS_RTOS == true && millis() -
         last_buttons_rtos_print >= 1000) {
        Serial << "buttons stack watermark: " 
               << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " 
               << xPortGetFreeHeapSize() << endl;
        last_buttons_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_BUTTONS);
    }
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_BUTTONS != 0) vTaskDelayUntil(&xLastWakeTime, TASK_FREQ_BUTTONS);
  }
  // task destructor prevents the task from doing damage 
  // to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setButtonsTaskPriority(uint8_t p) {

  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_BUTTONS);
  vTaskPrioritySet(Task_BUTTONS, p);
  if(DEBUG_BUTTONS_RTOS) Serial << "changed buttons task priority - new: " 
                                << p << " prev: " << prev_priority << endl;

}




