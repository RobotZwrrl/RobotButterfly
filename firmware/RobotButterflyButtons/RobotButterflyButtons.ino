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
// -------------------------------


// ----------- buttons -----------
enum ButtonStates {
  BUTTON_IDLE,
  BUTTON_RELEASED,
  BUTTON_PRESSED,
  BUTTON_LONG_HOLD
};

struct Button {
  uint8_t state;
  bool pressed;
  bool flag_pressed;
  bool flag_released;
  bool flag_press_notif;
  bool flag_notif;
  long press_time;
  long release_time;
  char name;
};

volatile static struct Button Button_L;
volatile static struct Button Button_R;

bool double_press = false;
long double_press_release_time = 0;
bool double_hold = false;
long double_hold_release_time = 0;
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


// --------------------------
// ---- Button Callbacks ----
// --------------------------

void buttonBothTouchdownCallback() {

}

void buttonLeftTouchdownCallback() {

}

void buttonRightTouchdownCallback() {

}


void buttonBothReleaseCallback() {

}

void buttonLeftReleaseCallback() {

}

void buttonRightReleaseCallback() {

}


void buttonBothHoldNotifCallback() { // give user feedback to release button
  // tone(BUZZER_PIN, NOTE_F5, 500);
  // noTone(BUZZER_PIN);
}

void buttonLeftHoldNotifCallback() { // give user feedback to release button
  // tone(BUZZER_PIN, NOTE_A5, 500);
  // noTone(BUZZER_PIN);
}

void buttonRightHoldNotifCallback() { // give user feedback to release button
  // tone(BUZZER_PIN, NOTE_A7, 500);
  // noTone(BUZZER_PIN);
}

void buttonBothHoldReleaseCallback() {

}

void buttonLeftHoldReleaseCallback() {

}

void buttonRightHoldReleaseCallback() {

}


/*
void buttonBothPressCallback() { // on release of both buttons
  noTone(BUZZER_PIN);
  delay(200);
  tone(BUZZER_PIN, NOTE_F5, 100);
  noTone(BUZZER_PIN);
  delay(200);
  tone(BUZZER_PIN, NOTE_F5, 100);
  noTone(BUZZER_PIN);
  delay(200);
}

void buttonLeftPressCallback() { // on touchdown
  tone(BUZZER_PIN, NOTE_A5, 100);
  noTone(BUZZER_PIN);
}

void buttonRightPressCallback() { // on touchdown
  tone(BUZZER_PIN, NOTE_A7, 100);
  noTone(BUZZER_PIN);
}

void buttonLeftReleaseCallback() { // on release
  Serial << ":: left released" << endl;
}

void buttonRightReleaseCallback() { // on release
  Serial << ":: right released" << endl;
}

*/

