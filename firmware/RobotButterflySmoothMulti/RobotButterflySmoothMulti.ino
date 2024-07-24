/*
 * Robot Butterfly Smooth Movements Multitask
 * -------------------------------------------------
 * Sample the HC-SR04 sensor in a separate task, transfer
 * the data via a queue. Timer to stop and restart the 
 * task if data not received before the core 0 watchdog 
 * restarts the ESP.
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 20 Jul 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <ESP32Servo.h> // 3.0.5 by Kevin Harrington, John K. Bennett
#define REFRESH_INTERVAL 1 // define this before including the ServoEasing library
#include "ServoEasing.hpp"


long last_print;


// ----------- servos -----------
static SemaphoreHandle_t semaphore_anim_frame;
static SemaphoreHandle_t semaphore_anim_loop;
static SemaphoreHandle_t semaphore_anim_done;

static TaskHandle_t Task_SM;
static QueueHandle_t Queue_SM1;   // struct Animation
static QueueHandle_t Queue_SM2;   // uint8_t ANIM_STATE

uint16_t valuesSM_count = 0;

ServoEasing s1;
ServoEasing s2;

struct ServoMotor {
  ServoEasing motor;
  uint16_t home_pos;
  uint16_t up_pos;
  uint16_t down_pos;
  uint16_t current_pos;
  uint16_t target_pos;
  bool active;
  bool calibrated;
  String name;
};

struct ServoMotor wing_left;
struct ServoMotor wing_right;

bool initialised_servos = false;
// ----------------------------------

// ----------- animation -----------
struct Animation {
  uint8_t id;
  uint16_t servo_L[ANIM_MAX];
  uint16_t servo_R[ANIM_MAX];
  uint8_t velocity[ANIM_MAX]; // degrees per second
  uint16_t dwell[ANIM_MAX];   // milliseconds
  long start;
  bool active;
  uint8_t frames;
  uint8_t index;
  bool loop;
  bool reverse;
  bool done;
};

struct Animation GentleFlap;
struct Animation HomeFrame;
// ----------------------------------


// 

hw_timer_t *timer1_cfg = NULL;
volatile bool callback_anim_frame = false;
volatile bool callback_anim_loop = false;
volatile bool callback_anim_done = false;

void IRAM_ATTR Timer1_ISR() {
  
}


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  // semaphore checker - set up timer 1 for every 50 ms
  timer1_cfg = timerBegin(16000);
  timerAttachInterrupt(timer1_cfg, &Timer1_ISR);
  // params: timer, tick count, auto-reload (true), reload count (0 = infinite)
  timerAlarm(timer1_cfg, 250, true, 0);
  timerStart(timer1_cfg);

  initAnimations();
  initServos(); // this also inits the queues

  Serial << "Ready" << endl;

}


void loop() {
  
  if(millis()-last_print >= 500) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

  updateAnimation();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'a':
        initServos();
      break;
      case '1':
        sendAnimation(&GentleFlap, 1);
      break;
      case '2':
        sendAnimation(&HomeFrame, 1);
      break;
      case 's':
        Serial << "stop" << endl;
        sendAnimation(&HomeFrame, 0);
      break;
    }
  }

}

