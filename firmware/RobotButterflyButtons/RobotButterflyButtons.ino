/*
 * Robot Butterfly Buttons
 * -------------------------
 * Buttons pressed via interrupts with debouncing and 
 * raising semaphores for other tasks
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


// tests
long last_print;
// --


// ----------- buttons -----------
static SemaphoreHandle_t Mutex_BT_L;
static SemaphoreHandle_t Mutex_BT_R;

enum ButtonStates {
  BUTTON_RELEASED,
  BUTTON_PRESSED,
  BUTTON_LONG_HOLD
};

struct Button {
  uint8_t state;
  bool pressed;
  bool printed_pressed;
  bool printed_released;
  long press_time;
  long release_time;
  char name;
};

volatile static struct Button Button_L;
volatile static struct Button Button_R;
// -----------------------------------

// ----------- buttons isr -----------
void IRAM_ATTR button_L_isr() {
  
  volatile struct Button *b = &Button_L;
  
  if(digitalRead(BUTTON2_PIN) == HIGH) { // pressed
    if(millis()-b->press_time <= DEBOUNCE_TIME && b->press_time > 0) { // debounce time
      return;
    }
    b->pressed = true;
    b->printed_pressed = false;
    b->press_time = millis();
  } else { // released
    if(millis()-b->release_time <= DEBOUNCE_TIME && b->release_time > 0) { // debounce time
      return;
    }
    b->printed_released = false;
    if(b->pressed == false) b->printed_released = true; // this prevents the soft release from triggering print twice
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
    b->pressed = true;
    b->printed_pressed = false;
    b->press_time = millis();
  } else { // released
    if(millis()-b->release_time <= DEBOUNCE_TIME && b->release_time > 0) { // debounce time
      return;
    }
    b->printed_released = false;
    if(b->pressed == false) b->printed_released = true; // this prevents the soft release from triggering print twice
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
  
  if(millis()-last_print >= 500) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

  updateButtons();

}


