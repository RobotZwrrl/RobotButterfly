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

 // @module: Buttons
 // @version: 0.1.0

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

static struct Button Button_L;
static struct Button Button_R;
// -----------------------------------


// -------- button callbacks ---------
void buttonHoldNotificationCallback(uint8_t n);
void buttonHoldReleasedCallback(uint8_t n);
void buttonClickCallback(uint8_t n);
void buttonReleaseCallback(uint8_t n);

typedef void (*ButtonCallback)(uint8_t); // button callback type
ButtonCallback onHoldNotificationCallback = NULL;
ButtonCallback onHoldReleasedCallback = NULL;
ButtonCallback onClickCallback = NULL;
ButtonCallback onReleaseCallback = NULL;
// -----------------------------------

volatile bool button_L_changed = false;
volatile bool button_R_changed = false;


// ----------- buttons isr -----------
void IRAM_ATTR button_L_isr() {
  button_L_changed = true;
}


void IRAM_ATTR button_R_isr() {
  button_R_changed = true;
}
// -----------------------------------


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  pinMode(BUZZER_PIN, OUTPUT);

  initButtons();

  Serial << "Ready" << endl;
}


void loop() {

  updateButtons();

}



void playSimpleTone(int freq, int duration) {
  int pwmVal = 127; // Half duty cycle
  int delayMicros = 1000000 / freq / 2;

  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(delayMicros);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(delayMicros);
  }
}

void playNoTone() {
  digitalWrite(BUZZER_PIN, LOW);
}
