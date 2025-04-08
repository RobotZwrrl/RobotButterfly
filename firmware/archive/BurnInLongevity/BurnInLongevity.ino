/*
 * Burn in / Longevity Test for Robot Butterfly
 * -------------------------------------
 * Move the wing servos for infinity and beyond!
 * Must have calibrated servos first
 * 
 * 11 Feb 2024 - Erin RobotZwrrl for Robot Missions Inc
 * www.robotmissions.org
 */

#include <ESP32Servo.h>
#include <Streaming.h>
#include "Board.h"

// adjust speed with these two vars
#define STEP_DEL   1
#define STEP_SIZE  10

Servo s1;
Servo s2;

struct ServoMotor {
  Servo motor;
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


void setup() {
  Serial.begin(9600);

  wing_left.motor = s1;
  wing_left.motor.attach(SERVO_LEFT_PIN);
  wing_left.active = true;
  wing_left.name = "LEFT";

  wing_right.motor = s2;
  wing_right.motor.attach(SERVO_RIGHT_PIN);
  wing_right.active = true;
  wing_right.name = "RIGHT";

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  digitalWrite(LED_HEARTBEAT_PIN, HIGH);

  if(SERVO_LEFT_HOME == -99) {
    Serial << "!! Servo LEFT needs calibration" << endl;
    wing_left.calibrated = false;
    wing_left.current_pos = 1500;
  } else {
    wing_left.home_pos = SERVO_LEFT_HOME;
    wing_left.up_pos = SERVO_LEFT_UP;
    wing_left.down_pos = SERVO_LEFT_DOWN;
    wing_left.motor.writeMicroseconds( SERVO_LEFT_UP );
  }

  if(SERVO_RIGHT_HOME == -99) {
    Serial << "!! Servo RIGHT needs calibration" << endl;
    wing_right.calibrated = false;
    wing_right.current_pos = 1500;
  } else {
    wing_right.home_pos = SERVO_RIGHT_HOME;
    wing_right.up_pos = SERVO_RIGHT_UP;
    wing_right.down_pos = SERVO_RIGHT_DOWN;
    wing_right.motor.writeMicroseconds( SERVO_RIGHT_UP );
  }

  Serial << "\r\n";

}

void loop() {

  // up to down
  for(uint16_t i=0; i<2000; i+=STEP_SIZE) {
    uint16_t left_map = map(i, 0, 2000, SERVO_LEFT_UP, SERVO_LEFT_DOWN);
    uint16_t right_map = map(i, 0, 2000, SERVO_RIGHT_UP, SERVO_RIGHT_DOWN);
    wing_left.current_pos = left_map;
    wing_right.current_pos = right_map;
    Serial << i << " : wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
    wing_left.motor.writeMicroseconds( wing_left.current_pos );
    wing_right.motor.writeMicroseconds( wing_right.current_pos );
    delay(STEP_DEL);
  }
  wing_left.motor.writeMicroseconds(SERVO_LEFT_DOWN);
  wing_right.motor.writeMicroseconds(SERVO_RIGHT_DOWN);
  
  delay(1000);

  // down to up
  for(uint16_t i=0; i<2000; i+=STEP_SIZE) {
    uint16_t left_map = map(i, 0, 2000, SERVO_LEFT_DOWN, SERVO_LEFT_UP);
    uint16_t right_map = map(i, 0, 2000, SERVO_RIGHT_DOWN, SERVO_RIGHT_UP);
    wing_left.current_pos = left_map;
    wing_right.current_pos = right_map;
    Serial << i << ": wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
    wing_left.motor.writeMicroseconds( wing_left.current_pos );
    wing_right.motor.writeMicroseconds( wing_right.current_pos );
    delay(STEP_DEL);
  }
  wing_left.motor.writeMicroseconds(SERVO_LEFT_UP);
  wing_right.motor.writeMicroseconds(SERVO_RIGHT_UP);

  delay(1000);
  

  uint16_t slice = millis() % 1000;
  digitalWrite(LED_HEARTBEAT_PIN, slice < 100 || (slice > 200 && slice < 300));

}

