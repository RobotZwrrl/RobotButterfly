#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"

// hi!
struct Button;

// ----------- callbacks -----------
typedef void (*ButtonCallback)(uint8_t);

extern ButtonCallback onHoldNotificationCallback;
extern ButtonCallback onHoldReleasedCallback;
extern ButtonCallback onClickCallback;
extern ButtonCallback onReleaseCallback;
// -----------------------------------

void initButtons();
void updateButtons();
void Task_BUTTONS_code(void * pvParameters);
void setButtonsTaskPriority(uint8_t p);

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

static volatile bool button_L_changed;
static volatile bool button_R_changed;

// ---------- rtos ------------
static TaskHandle_t Task_BUTTONS;
static SemaphoreHandle_t Mutex_BUTTONS;
static long last_buttons_rtos_print;
// -------------------------------

#endif