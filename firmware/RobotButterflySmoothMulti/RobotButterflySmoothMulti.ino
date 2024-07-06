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
 * 6 Jul 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <ESP32Servo.h> //3.0.5 by Kevin Harrington, John K. Bennett
#include "ServoEasing.hpp"

hw_timer_t *timer0_cfg = NULL;
long last_print;


// ----------- servos -----------
TaskHandle_t Task_SM;
QueueHandle_t Queue_SM1;
QueueHandle_t Queue_SM2;
QueueHandle_t Queue_SM3;

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
struct Keyframe {
  uint16_t servo_L;
  uint16_t servo_R;
  uint8_t velocity; // degrees per second
  uint16_t dwell; // milliseconds
  long start;
  bool moving;
};

struct Keyframe *allocateFrames(int count) {
  struct Keyframe *frames = (struct Keyframe *)malloc(sizeof(struct Keyframe)*count);
  return frames;
}

const uint8_t flap_steps = 2;
uint8_t flap_index = 0;
struct Keyframe *FlapFrames = allocateFrames(flap_steps);

const uint8_t home_steps = 1;
uint8_t home_index = 0;
struct Keyframe *HomeFrames = allocateFrames(home_steps);

struct Keyframe *Animation = allocateFrames(ANIM_MAX);
// ----------------------------------

void IRAM_ATTR Timer0_ISR() {
  //updateUS = true;
}


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  // data digester - set up timer 0 for every 0.5 second
  timer0_cfg = timerBegin(16000);
  timerAttachInterrupt(timer0_cfg, &Timer0_ISR);
  // params: timer, tick count, auto-reload (true), reload count (0 = infinite)
  timerAlarm(timer0_cfg, 2500, true, 0);
  timerStart(timer0_cfg);

  initAnimations();
  initServos(); // this also inits the queues
  loadAnimation(HomeFrames, home_steps, home_index);

  Serial << "Ready" << endl;

}


void loop() {
  
  if(millis()-last_print >= 500) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

  //updateUltrasonic();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'a':
        initServos();
      break;
      case '1':
        loadAnimation(FlapFrames, flap_steps, flap_index);
        Serial << "dwell time set: " << FlapFrames[0].dwell << endl;
      break;
      case '2':
        loadAnimation(HomeFrames, home_steps, home_index);
        Serial << "dwell time set: " << HomeFrames[0].dwell << endl;
      break;
      case '3':
      break;
    }
  }

}

