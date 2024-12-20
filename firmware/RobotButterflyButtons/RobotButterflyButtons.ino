/*
 * Robot Butterfly Buttons
 * -------------------------
 * Buttons pressed via interrupts with debouncing
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 18 Aug 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include "pitches.h"


// ------------ tests ------------
long last_print;
int counter = 0;
// -------------------------------


// ----------- buttons -----------
enum ButtonNames {
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_BOTH
};

enum ButtonStates {
  BUTTON_IDLE,
  BUTTON_RELEASED,
  BUTTON_BOTH_HOLD,
  BUTTON_HOLD,
  BUTTON_PRESSED,
  BUTTON_CLICK
};

struct Button {
  uint8_t state;
  uint8_t state_prev;
  bool pressed;
  bool flag_pressed;
  bool flag_released;
  bool flag_state_change;
  bool flag_button_hold_notif;
  long press_time;
  long release_time;
  long release_both_time;
  long click_time;
  char name;
};

volatile static struct Button Button_L;
volatile static struct Button Button_R;
// -----------------------------------


// -------- button callbacks ---------
void buttonHoldNotificationCallback(uint8_t n);
void buttonHoldReleasedCallback(uint8_t n);
void buttonClickCallback(uint8_t n);
void buttonReleaseCallback(uint8_t n);
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

  Serial << "Ready" << endl;
}



void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateButtons();
}


// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------

// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      tone(BUZZER_PIN, NOTE_F5, 500);
      noTone(BUZZER_PIN);
      tone(BUZZER_PIN, 0, 500);
      noTone(BUZZER_PIN);
    break;
    case BUTTON_LEFT:
      tone(BUZZER_PIN, NOTE_A5, 500);
      noTone(BUZZER_PIN);
    break;
    case BUTTON_RIGHT:
      tone(BUZZER_PIN, NOTE_A7, 500);
      noTone(BUZZER_PIN);
    break;
  }
}

// do an action here
void buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      counter *= 2;
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
      tone(BUZZER_PIN, NOTE_A5, 100);
      noTone(BUZZER_PIN);
      counter++;
    break;
    case BUTTON_RIGHT:
      tone(BUZZER_PIN, NOTE_A7, 100);
      noTone(BUZZER_PIN);
      counter--;
    break;
  }
  Serial << "count: " << counter << endl;
}

// probably not necessary to do anything here
void buttonReleaseCallback(uint8_t n) {

}


