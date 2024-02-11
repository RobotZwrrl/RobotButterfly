/*
 * Calibrate Servos for Robot Butterfly
 * -------------------------------------
 * Use this code to set the home, up, and down positions
 * for the two wing servos on Robot Butterfly
 * 
 * 11 Feb 2024 - Erin RobotZwrrl for Robot Missions Inc
 * www.robotmissions.org
 */

#include <ESP32Servo.h>
#include <Streaming.h>
#include "Board.h"

#define STEP_SIZE_DEFAULT 100

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

enum State {
  IDLE,
  CALIB_LEFT,
  CALIB_RIGHT,
  TEST
};

enum State CURRENT_STATE = IDLE;

enum Step {
  START,
  UP,
  DOWN,
  HOME
};

enum Step CURRENT_STEP = START;

uint8_t step_size = STEP_SIZE_DEFAULT;
long last_print = 0;
bool fp = false;
long last_update = 0;


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

  Serial << "Press h to start" << endl;

  while(!Serial.available()) {;} // wait for terminal

  Serial << "\r\n\r\n\r\n";

  if(SERVO_LEFT_HOME == -99) {
    Serial << "!! Servo LEFT needs calibration" << endl;
    wing_left.calibrated = false;
    wing_left.current_pos = 1500;
  } else {
    wing_left.home_pos = SERVO_LEFT_HOME;
    wing_left.up_pos = SERVO_LEFT_UP;
    wing_left.down_pos = SERVO_LEFT_DOWN;
    wing_left.motor.writeMicroseconds( SERVO_LEFT_HOME );
  }

  if(SERVO_RIGHT_HOME == -99) {
    Serial << "!! Servo RIGHT needs calibration" << endl;
    wing_right.calibrated = false;
    wing_right.current_pos = 1500;
  } else {
    wing_right.home_pos = SERVO_RIGHT_HOME;
    wing_right.up_pos = SERVO_RIGHT_UP;
    wing_right.down_pos = SERVO_RIGHT_DOWN;
    wing_right.motor.writeMicroseconds( SERVO_RIGHT_HOME );
  }

  Serial << "\r\n";

}

void loop() {

  switch(CURRENT_STATE) {
    case IDLE: {

      if(Serial.available() > 0) {
        char c = Serial.read();
        switch(c) {
          case 'l':
            Serial << "Calibrating left" << endl;
            CURRENT_STATE = CALIB_LEFT;
          break;
          case 'r':
            Serial << "Calibrating right" << endl;
            CURRENT_STATE = CALIB_RIGHT;
          break;
          case 'p':
            printResults(&wing_left);
            printResults(&wing_right);
          break;
          case 't':
            if(CURRENT_STATE == IDLE) {
              CURRENT_STATE = TEST;
            } else {
              CURRENT_STATE = IDLE;
            }
          break;
          case 'h':
            mainMenuHelp();
          break;
        }
      }

    }
    break;
    case CALIB_LEFT: 
      calibrate(&wing_left);
    break;
    case CALIB_RIGHT: 
      calibrate(&wing_right);
    break;
    case TEST:

      // home to up
      for(uint16_t i=0; i<500; i+=10) {
        uint16_t left_map = map(i, 0, 500, SERVO_LEFT_HOME, SERVO_LEFT_UP);
        uint16_t right_map = map(i, 0, 500, SERVO_RIGHT_HOME, SERVO_RIGHT_UP);
        wing_left.current_pos = left_map;
        wing_right.current_pos = right_map;
        Serial << i << " : wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
        wing_left.motor.writeMicroseconds( wing_left.current_pos );
        wing_right.motor.writeMicroseconds( wing_right.current_pos );
        delay(5);
      }
      wing_left.motor.writeMicroseconds(SERVO_LEFT_UP);
      wing_right.motor.writeMicroseconds(SERVO_RIGHT_UP);

      delay(1000);

      // up to down
      for(uint16_t i=0; i<1000; i+=10) {
        uint16_t left_map = map(i, 0, 1000, SERVO_LEFT_UP, SERVO_LEFT_DOWN);
        uint16_t right_map = map(i, 0, 1000, SERVO_RIGHT_UP, SERVO_RIGHT_DOWN);
        wing_left.current_pos = left_map;
        wing_right.current_pos = right_map;
        Serial << i << ": wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
        wing_left.motor.writeMicroseconds( wing_left.current_pos );
        wing_right.motor.writeMicroseconds( wing_right.current_pos );
        delay(5);
      }
      wing_left.motor.writeMicroseconds(SERVO_LEFT_DOWN);
      wing_right.motor.writeMicroseconds(SERVO_RIGHT_DOWN);

      delay(1000);
      
      // down to home
      for(uint16_t i=0; i<500; i+=10) {
        uint16_t left_map = map(i, 0, 500, SERVO_LEFT_DOWN, SERVO_LEFT_HOME);
        uint16_t right_map = map(i, 0, 500, SERVO_RIGHT_DOWN, SERVO_RIGHT_HOME);
        wing_left.current_pos = left_map;
        wing_right.current_pos = right_map;
        Serial << i << " : wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
        wing_left.motor.writeMicroseconds( wing_left.current_pos );
        wing_right.motor.writeMicroseconds( wing_right.current_pos );
        delay(5);
      }
      wing_left.motor.writeMicroseconds(SERVO_LEFT_HOME);
      wing_right.motor.writeMicroseconds(SERVO_RIGHT_HOME);

      Serial << "\r\nTest complete!\r\n" << endl;

      CURRENT_STATE = IDLE;

    break;
  }

  uint16_t slice = millis() % 1000;
  digitalWrite(LED_HEARTBEAT_PIN, slice < 100 || (slice > 200 && slice < 300));

}


void calibrate(struct ServoMotor *s) {

  switch(CURRENT_STEP) {
    case START:
      Serial << "This wizard will calibrate the up, down, and home position" << endl;
      helpInstructions();
      fp = true;
      CURRENT_STEP = UP;
    break;
    case UP:
      if(millis()-last_print >= 30000 || fp == true) {
        Serial << "\r\n";
        Serial << "Calibrating the UP position" << endl;
        Serial << " - the wing should be straight up and not colliding into anything" << endl;
        Serial << "Press h for instructions, g to set position" << endl;
        Serial << "\r\n";
        last_print = millis();
        fp = false;
      }
    break;
    case DOWN:
      if(millis()-last_print >= 30000 || fp == true) {
        Serial << "\r\n";
        Serial << "Calibrating the DOWN position" << endl;
        Serial << " - the wing should be not touching the table or colliding with its pieces" << endl;
        Serial << "Press h for instructions, g to set position" << endl;
        Serial << "\r\n";
        last_print = millis();
        fp = false;
      }
    break;
    case HOME:
      if(millis()-last_print >= 30000 || fp == true) {
        Serial << "\r\n";
        Serial << "Calibrating the HOME position";
        Serial << " - the wing should be flat horizontal" << endl;
        Serial << "Press h for instructions, g to set position" << endl;
        Serial << "\r\n";
        last_print = millis();
        fp = false;
      }
    break;
  }

  if(millis()-last_update >= 500) {
    s->motor.writeMicroseconds( s->current_pos );
    last_update = millis();
  }

  if(Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'l':
        Serial << "\r\n";
        Serial << "Exiting calibration" << endl;
        step_size = STEP_SIZE_DEFAULT;
        fp = true;
        CURRENT_STATE = IDLE;
        mainMenuHelp();
      break;
      case 'a':
        s->current_pos += step_size;
        s->motor.writeMicroseconds( s->current_pos );
        Serial << s->name << " position: " << s->current_pos << endl;
      break;
      case 'd':
        s->current_pos -= step_size;
        s->motor.writeMicroseconds( s->current_pos );
        Serial << s->name << " position: " << s->current_pos << endl;
      break;
      case 'w':
        if(step_size < 225) step_size += 25;
        Serial << "step size increased to " << step_size << " (more coarse)" << endl;
      break;
      case 's':
        if(step_size > 25) step_size -= 25;
        Serial << "step size decreased to " << step_size << " (more fine)" << endl;
      break;
      case 'p':
        printResults(s);
      break;
      case 'g':
        
        if(CURRENT_STEP == UP) s->up_pos = s->current_pos;
        if(CURRENT_STEP == DOWN) s->down_pos = s->current_pos;
        if(CURRENT_STEP == HOME) s->home_pos = s->current_pos;

        Serial << "\r\n";

        if(CURRENT_STEP == HOME) {
          Serial << "Awesome! Calibration complete" << endl;
          printResults(s);
          fp = true;
          step_size = STEP_SIZE_DEFAULT;
          CURRENT_STEP = START;
          CURRENT_STATE = IDLE;
        } else {
          Serial << "Great! Let's move on to the next step" << endl;
          fp = true;
          step_size = STEP_SIZE_DEFAULT;
          incrementCurrentStep();
        }

      break;
    }
  }

}


void printResults(struct ServoMotor *s) {

  Serial << "-------------------------" << endl;
  Serial << "\r\n";

  Serial << "Printing results for " << s->name << " servo" << endl;
  Serial << "Please paste these results into your Board.h file" << endl;
  Serial << "\r\n";

  Serial << "#define SERVO_" << s->name << "_HOME    " << s->home_pos << endl;
  Serial << "#define SERVO_" << s->name << "_UP      " << s->up_pos << endl;
  Serial << "#define SERVO_" << s->name << "_DOWN    " << s->down_pos << endl;

  Serial << "\r\n";
  Serial << "-------------------------" << endl;

}


void helpInstructions() {
  Serial << "\r\n";
  Serial << "Press g to set the position" << endl;
  Serial << "Press a to increment" << endl;
  Serial << "Press d to decrement" << endl;
  Serial << "Press w to increase step size" << endl;
  Serial << "Press s to decrease step size" << endl;
  Serial << "Press p to print results" << endl;
  Serial << "Press l or r to exit without calibrating" << endl;
  Serial << "Press h to display these instructions" << endl;
  Serial << "\r\n";
}


void mainMenuHelp() {
  Serial << "Press l to calibrate left servo" << endl;
  Serial << "Press r to calibrate right servo" << endl;
  Serial << "Press p to print results" << endl;
  Serial << "Press t to test with movement" << endl;
  Serial << "\r\n";
}


void incrementCurrentStep() {

  if(CURRENT_STEP == START) {
    CURRENT_STEP = UP;
    return;
  }

  if(CURRENT_STEP == UP) {
    CURRENT_STEP = DOWN;
    return;
  }

  if(CURRENT_STEP == DOWN) {
    CURRENT_STEP = HOME;
    return;
  }
  
}

