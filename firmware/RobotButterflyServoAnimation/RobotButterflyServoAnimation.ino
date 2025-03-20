/*
 * Robot Butterfly Servo Animation
 * --------------------------------
 * Servo animations
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 18 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <ESP32Servo.h>      // 3.0.6 https://github.com/madhephaestus/ESP32Servo 
#include "ServoEasing.hpp"   // 3.4.0 https://github.com/ArminJo/ServoEasing



// ----------- servo motors -----------
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
// -----------------------------------


// ----------- servo animation -----------
enum servoAnimName {
  SERVO_ANIM_NONE,
  SERVO_ANIM_GENTLEFLAP,
  SERVO_ANIM_SWAY,
  SERVO_ANIM_SOARING,
  SERVO_ANIM_TOUCHGRASS
};

enum servoAnimType {
  SERVO_ANIM_ALERT,
  SERVO_ANIM_HOME
};

void runServoAnim_none(struct ServoAnimation *animation);
void runServoAnim_gentleflap(struct ServoAnimation *animation);
void runServoAnim_sway(struct ServoAnimation *animation);
void runServoAnim_soaring(struct ServoAnimation *animation);
void runServoAnim_touchgrass(struct ServoAnimation *animation);
typedef void (*ServoAnimationFunction)(ServoAnimation*); // function pointer type that accepts a ServoAnimation pointer

struct ServoAnimation {
  uint8_t id;
  bool active;
  uint8_t type;
  uint8_t velocity;   // degrees per second

  uint8_t num_frames;
  uint16_t frame_delay;
  int frame_index;
  long last_frame;
  
  int num_repeats;
  uint16_t repeat_count;
  uint16_t repeat_delay;
  long last_repeat;

  long duration;
  long start_time;

  bool dir;
  int helper1;
  int helper2;
  int helper3;

  ServoAnimationFunction function;  // function pointer
};

ServoAnimation servo_animation_home;
ServoAnimation servo_animation_alert;
// -----------------------------------



void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initServos();
  initServoAnimations();

  Serial << "Ready" << endl;
}


void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateServoAnimation();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '0':
        setServoAnim(&servo_animation_alert, SERVO_ANIM_NONE, SERVO_ANIM_ALERT);
        startServoAnim(&servo_animation_alert);
      break;
      case '1':
        Serial << "Gentle flap default" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_GENTLEFLAP, SERVO_ANIM_ALERT);
        startServoAnim(&servo_animation_alert);
      break;
      case '2':
        Serial << "Sway default" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
        startServoAnim(&servo_animation_alert);
      break;
      case '3':
        Serial << "Gentle flap 5 seconds and faster velocity" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_GENTLEFLAP, SERVO_ANIM_ALERT);
        setServoAnimDuration(&servo_animation_alert, 5000);
        setServoAnimSpeed(&servo_animation_alert, 80);
        startServoAnim(&servo_animation_alert);
      break;
      case '4':
        Serial << "Sway 5 seconds and faster velocity" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
        setServoAnimDuration(&servo_animation_alert, 5000);
        setServoAnimSpeed(&servo_animation_alert, 100);
        startServoAnim(&servo_animation_alert);
      break;
      case '5':
        Serial << "Gentle flap 3x" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_GENTLEFLAP, SERVO_ANIM_ALERT);
        setServoAnimRepeats(&servo_animation_alert, 3);
        startServoAnim(&servo_animation_alert);
      break;
      case '6':
        Serial << "Sway 3x" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
        setServoAnimRepeats(&servo_animation_alert, 3);
        startServoAnim(&servo_animation_alert);
      break;
      case '7':
        Serial << "Soaring (default)" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_SOARING, SERVO_ANIM_ALERT);
        startServoAnim(&servo_animation_alert);
      break;
      case '8':
        Serial << "Soaring faster velocity" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_SOARING, SERVO_ANIM_ALERT);
        setServoAnimSpeed(&servo_animation_alert, 200);
        startServoAnim(&servo_animation_alert);
      break;
      case '9':
        Serial << "Touch grass (default)" << endl;
        setServoAnim(&servo_animation_alert, SERVO_ANIM_TOUCHGRASS, SERVO_ANIM_ALERT);
        //setServoAnimSpeed(&servo_animation_alert, 200);
        startServoAnim(&servo_animation_alert);
      break;
      case 's':
        stopServoAnim(&servo_animation_alert);
      break;
      case 'h':
        Serial << "1-9: animations" << endl;
        Serial << "s: stop" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}

