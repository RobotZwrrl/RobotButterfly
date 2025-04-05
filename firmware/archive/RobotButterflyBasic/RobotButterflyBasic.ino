/*
 * Robot Butterfly Basic
 * -------------------------------------
 * Set the wing servos to their upright position
 * Left button = Calm flapping behaviour
 * Right button = Distance behaviour
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 *
 * 3 May 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "pitches.h"
#include <ESP32Servo.h>
#include <Streaming.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include <HCSR04.h>     // HCSR04 library by Dirk Sarodnick
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg


#define BATT_OP true    // optional delays to not brown out the robot on startup

#if (BATT_OP == true)
  movingAvg ultrasonicAvg(10);
#else
  movingAvg ultrasonicAvg(5);
#endif

Bounce button_L = Bounce();
Bounce button_R = Bounce();

Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// -- servo objects
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
// --------

// -- states
enum States {
  IDLE,
  FLAP,
  DIST
}; 
enum States STATE = IDLE;

enum FlapSteps {
  STEP1,
  STEP2
};
enum FlapSteps FLAP_STEP = STEP1;
// --------

// delays & flags
long last_dist_servo = 0;
long last_hello_print = 0;
long last_sparkle = 0;
long last_step = 0;
bool both = false;

// behaviours
void gentleFlap();
void distanceFlap();

// neopixels
void patternPurplePink(uint8_t i);
void neoSparkle();
void neoUpdate();
void playMelody();


void setup() {
  Serial.begin(9600);

  HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);//, 100, HCSR04Sensor::eUltraSonicUnlock_t::unlockSkip);
  ultrasonicAvg.begin();

  button_L.attach(BUTTON1_PIN, INPUT);
  button_L.interval(5);
  button_R.attach(BUTTON2_PIN, INPUT);
  button_R.interval(5);

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  digitalWrite(LED_HEARTBEAT_PIN, HIGH);
  pinMode(LED_COMMS_PIN, OUTPUT);
  digitalWrite(LED_COMMS_PIN, LOW);

  strip.begin();
  strip.setBrightness(10);
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(10, 10, 10));
  }
  strip.show();

  wing_left.motor = s1;
  wing_left.motor.attach(SERVO_LEFT_PIN);
  wing_left.active = true;
  wing_left.name = "LEFT";

  wing_right.motor = s2;
  wing_right.motor.attach(SERVO_RIGHT_PIN);
  wing_right.active = true;
  wing_right.name = "RIGHT";

  if(SERVO_LEFT_HOME == -99) {
    Serial << "!! Servo LEFT needs calibration" << endl;
    wing_left.calibrated = false;
  }

  wing_left.home_pos = SERVO_LEFT_HOME;
  wing_left.up_pos = SERVO_LEFT_UP;
  wing_left.down_pos = SERVO_LEFT_DOWN;
  wing_left.current_pos = SERVO_LEFT_UP;
  wing_left.motor.writeMicroseconds( SERVO_LEFT_UP );

  if(SERVO_RIGHT_HOME == -99) {
    Serial << "!! Servo RIGHT needs calibration" << endl;
    wing_right.calibrated = false;
  }

  wing_right.home_pos = SERVO_RIGHT_HOME;
  wing_right.up_pos = SERVO_RIGHT_UP;
  wing_right.down_pos = SERVO_RIGHT_DOWN;
  wing_right.motor.writeMicroseconds( SERVO_RIGHT_UP );

  Serial << "\r\n";

}


void loop() {

  buttonUpdate();
  heartbeatUpdate();

  switch(STATE) {
    case IDLE: {
      wing_left.motor.writeMicroseconds( SERVO_LEFT_UP );
      wing_right.motor.writeMicroseconds( SERVO_RIGHT_UP );
      neoUpdate();
      if(millis()-last_hello_print >= 5000) {
        Serial << millis() << " Hello Robot Butterfly! ^_^";
        // Serial << " CPU: " << getCpuFrequencyMhz() << " MHz";
        // Serial << " XTAL: " << getXtalFrequencyMhz() << " MHz";
        Serial << endl;
        last_hello_print = millis();        
      }
    }
    break;
    case FLAP: {
      gentleFlap();
      neoUpdate();
    }
    break;
    case DIST: {
      distanceFlap();
    }
    break;
  }

}


void buttonUpdate() {

  button_L.update();
  button_R.update();

  if(button_L.changed() == true || button_R.changed() == true) {
    if(button_L.read() == HIGH && button_R.read() == HIGH) {
      STATE = IDLE;
      playMelody();
      both = true;
    }
  }

  if(button_L.changed()) {
    if(button_L.read() == LOW) { // released
      if(both) { // flag set by both buttons pressed
        both = false;
        return;
      }
      Serial << "button_L LOW (released)" << endl;
      if(STATE == FLAP) {
        STATE = IDLE;
      } else {
        STATE = FLAP;
      }
      Serial << "STATE = " << STATE << endl;
      for(uint8_t i=0; i<3; i++) {
        tone(BUZZER_PIN, NOTE_E3, 80);
        tone(BUZZER_PIN, NOTE_C4, 80);
      }
      noTone(BUZZER_PIN);
    }
  }

  if(button_R.changed()) {
    if(button_R.read() == LOW) { // released
      if(both) { // flag set by both buttons pressed
        both = false;
        return;
      }
      Serial << "button_R LOW (released)" << endl;
      if(STATE == DIST) {
        STATE = IDLE;
      } else {
        STATE = DIST;
      }
      Serial << "STATE = " << STATE << endl;
      for(uint8_t i=0; i<3; i++) {
        tone(BUZZER_PIN, NOTE_G3, 80);
        tone(BUZZER_PIN, NOTE_D2, 80);
      }
      noTone(BUZZER_PIN);
    }
  }


}


void heartbeatUpdate() {
  uint16_t slice = millis() % 1000;
  digitalWrite(LED_HEARTBEAT_PIN, slice < 100 || (slice > 200 && slice < 300));
}




