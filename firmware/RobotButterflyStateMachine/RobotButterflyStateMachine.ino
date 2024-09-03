/*
 * Robot Butterfly State Machine
 * ------------------------------
 * Go through various states
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 25 Aug 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>


// ------------ tests ------------
long last_print;
// -------------------------------

// ------------ states -----------
enum StatesMachine { // this is where you can order the behaviours
  STATE1,
  STATE2,
  STATE3,
  STATE4,
  STATE5,
  STATE6,
  STATE7,
  STATE8
};

uint8_t CURRENT_STATE = STATE1; // this is where you can set the initial behaviour
uint8_t PREV_STATE = CURRENT_STATE;
long last_state_change = 0;
long last_state_print = 0;

volatile bool enter_state = false;
bool transition_state = false;
bool update_state = true;
long last_transition_print = 0;

hw_timer_t *timer1_cfg = NULL;
void IRAM_ATTR Timer1_ISR() {
  enter_state = true;
}
// -------------------------------

// ----------- buttons -----------
enum ButtonStates {
  BUTTON_RELEASED,
  BUTTON_PRESSED,
  BUTTON_LONG_HOLD
};

struct Button {
  uint8_t state;
  bool pressed;
  bool flag_pressed;
  bool flag_released;
  long press_time;
  long release_time;
  char name;
};

volatile static struct Button Button_L;
volatile static struct Button Button_R;

bool double_hold = false;
// -----------------------------------

// ----------- buttons isr -----------
void IRAM_ATTR button_L_isr() {
  
  volatile struct Button *b = &Button_L;
  
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
}


void IRAM_ATTR button_R_isr() {

  volatile struct Button *b = &Button_R;
  
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
}
// -----------------------------------



void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initButtons();

  // TODO: get the time to be TRANSITION_FRAME_TIME
  // timer transition - timer 1
  timer1_cfg = timerBegin(80000);
  timerAttachInterrupt(timer1_cfg, &Timer1_ISR);
  // params: timer, tick count, auto-reload (false to run once), reload count (0 = infinite)
  timerAlarm(timer1_cfg, TRANSITION_FRAME_TIME, false, 0);
  timerStart(timer1_cfg);
  timerStop(timer1_cfg);
  
  Serial << "Ready" << endl;
}



void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateButtons();

  // state machine init
  if(enter_state) {
    //timerEnd(timer1_cfg);
    Serial << millis() << " enter state" << endl;
    transition_state = false;
    enter_state = false;
    update_state = true;

    switch(CURRENT_STATE) {
      case STATE1:
        Serial << "STATE1 init" << endl;
      break;
      case STATE2:
        Serial << "STATE2 init" << endl;
      break;
      case STATE3:
        Serial << "STATE3 init" << endl;
      break;
      case STATE4:
        Serial << "STATE4 init" << endl;
      break;
      case STATE5:
        Serial << "STATE5 init" << endl;
      break;
      case STATE6:
        Serial << "STATE6 init" << endl;
      break;
      case STATE7:
        Serial << "STATE7 init" << endl;
      break;
      case STATE8:
        Serial << "STATE8 init" << endl;
      break;
    }

  }

  if(transition_state) {
    //if(millis()-last_transition_print >= TRANSITION_FRAME_TIME-100 || last_transition_print == 0) {
      Serial << millis() << " getting ready to transition to state index: " << CURRENT_STATE << endl;
      for(uint8_t i=0; i<CURRENT_STATE+1; i++) {
        Serial << "*";
      }
      Serial << endl;
      last_transition_print = millis();
    //}
  }


  // state machine loop
  if(update_state) {
    switch(CURRENT_STATE) {
      case STATE1:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE1 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE2:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE2 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE3:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE3 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE4:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE4 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE5:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE5 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE6:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE6 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE7:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE7 loop" << endl;
          last_state_print = millis();
        }
      break;
      case STATE8:
        if(millis()-last_state_print >= STATE_LOOP_PRINT) {
          Serial << "STATE8 loop" << endl;
          last_state_print = millis();
        }
      break;
    }
  }


}



