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


// ------------ states -----------
// this is where you can order the behaviours
enum StatesMachine {
  STATE1,
  STATE2,
  STATE3,
  STATE4,
  STATE5,
  STATE6,
  STATE7,
  STATE8
};

uint8_t CURRENT_STATE;
uint8_t PREV_STATE;
long last_state_change = 0;
long last_state_print = 0;

volatile bool enter_state = false;
volatile bool update_state = false;
volatile bool new_print = false;
long t_transition = 0;
long t_enter = 0;
long t_delta = 0;

hw_timer_t *timer_state_cfg = NULL;

void IRAM_ATTR Timer_State_ISR() {
  enter_state = false;
  update_state = true;
  new_print = true;
}
// -------------------------------

// ------------ other ------------
long last_print;
// -------------------------------


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  changeState(STATE1); // this is where you can set the initial state

  Serial << "Ready" << endl;
}


void loop() {

  console();

  // state entrance
  if(enter_state) {
    t_enter = millis();
    //t_delta = t_enter - t_transition;
    //if(DEBUG_STATEMACHINE) Serial << "enter: " << t_enter << " - transition: " << t_transition << " = delta: " << t_delta << endl;

    // only have the state entrance happen once
    enter_state = false;

    // if(timer_state_cfg != NULL) {
    //   if(DEBUG_STATEMACHINE) Serial << "timer done" << endl;
    //   timerEnd(timer_state_cfg);
    // }

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

  
  // state machine loop
  if(update_state) {
    switch(CURRENT_STATE) {
      case STATE1:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE1 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE2:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE2 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE3:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE3 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE4:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE4 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE5:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE5 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE6:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE6 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE7:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE7 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
      case STATE8:
        if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
          Serial << "STATE8 loop" << endl;
          new_print = false;
          last_state_print = millis();
        }
      break;
    }
  }


}


void console() {

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '1':
        changeState(STATE1);
      break;
      case '2':
        changeState(STATE2);
      break;
      case '3':
        changeState(STATE3);
      break;
      case '4':
        changeState(STATE4);
      break;
      case '5':
        changeState(STATE5);
      break;
      case '6':
        changeState(STATE6);
      break;
      case '7':
        changeState(STATE7);
      break;
      case '8':
        changeState(STATE8);
      break;

      case 'a':
        decrementState();
      break;
      case 'd':
        incrementState();
      break;

      case 'h':
        Serial << "1-8: states" << endl;
        Serial << "a: r button" << endl;
        Serial << "d: l button" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}



