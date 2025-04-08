/*
 * Robot Butterfly Buttons Multi
 * ------------------------------
 * Buttons pressed via interrupts with debouncing
 * Updates in a task with FreeRTOS
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2 or 2.0.4
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
static TaskHandle_t Task_BUTTONS;
static SemaphoreHandle_t Mutex_BUTTONS;

struct ButtonTaskMon {
  long task_enter;
  long task_exit;
  uint8_t task_priority;
};

struct ButtonTaskMon buttonTaskMon;

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

long last_buzz_both = 0;  // for a non-blocking delay for sound when both buttons are held
bool buzz_dir = true;     // alternate between tone and noTone
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

  if(Serial.available()) {
    char c = Serial.read();
    Serial << "priority is: " << uxTaskPriorityGet(Task_BUTTONS) << endl;
    switch(c) {
      case '[':
        vTaskPrioritySet(Task_BUTTONS, PRIORITY_BUTTONS_LOW);
        Serial << "PRIORITY_BUTTONS_LOW" << endl;
      break;
      case ']':
        vTaskPrioritySet(Task_BUTTONS, PRIORITY_BUTTONS_HIGH);
        Serial << "PRIORITY_BUTTONS_HIGH" << endl;
      break;
      case '\\':
        vTaskPrioritySet(Task_BUTTONS, tskIDLE_PRIORITY);
        Serial << "tskIDLE_PRIORITY" << endl;
      break;
      case 'p': {
        uint8_t p = buttonTaskMon.task_priority;
        long t1 = buttonTaskMon.task_enter;
        long t2 = buttonTaskMon.task_exit;
        Serial << "Task priority: " << p << endl;
        Serial << "Task enter: " << t1 << endl;
        Serial << "Task exit: " << t2 << endl;
        Serial << "Task time: " << t2-t1 << endl;
      }
      break;
      case 'h':
        Serial << "[: set priority low" << endl;
        Serial << "]: set priority high" << endl;
        Serial << "\\: set priority idle" << endl;
        Serial << "p: task mon" << endl;
      break;
    }
  }

  //updateButtons();
}


// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------


// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      if(millis()-last_buzz_both >= 200) {
        if(buzz_dir) {
          tone(BUZZER_PIN, NOTE_F5, 500);
        } else {
          noTone(BUZZER_PIN);
        }
        buzz_dir = !buzz_dir;
        last_buzz_both = millis();
      }
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


