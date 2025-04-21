/*
 * Robot Butterfly State Machine
 * ------------------------------
 * Go through various states, uses timer 3 for state 
 * entrance transition
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

// @module: StateMachine
// @version: 0.1.0

#include "Board.h"
#include "Params.h"
#include <Streaming.h>


// ------------ states -----------
// this is where you can order the behaviours
// @globals_start
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
volatile bool new_enter = false;
volatile bool new_update = false;
long t_transition = 0;
long t_enter = 0;
long t_delta = 0;

hw_timer_t *timer_state_cfg = NULL;
// @globals_end

// @isr_start
void IRAM_ATTR Timer_State_ISR() {
  enter_state = false;
  update_state = true;
  new_print = true;
  new_update = true;
}
// @isr_end
// -------------------------------

// ------------ other ------------
long last_print;
bool BATTERY_AA_MODE = false;
// -------------------------------


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initStateMachine();
  changeState(STATE1); // this is where you can set the initial state

  Serial << "Ready" << endl;
}


void loop() {

  console();

  updateStateMachine();

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



