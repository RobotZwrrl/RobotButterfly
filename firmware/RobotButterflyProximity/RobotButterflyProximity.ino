/*
 * Robot Butterfly Proximity
 * ---------------------------
 * Measure distance with the HC-SR04 sensor
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 27 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <movingAvg.h>   // 2.3.2 https://github.com/JChristensen/movingAvg
#include <HCSR04.h>      // 1.1.3 https://github.com/d03n3rfr1tz3/HC-SR04 


// ------------ callbacks ------------
void proximityTriggerCallback(struct Proximity *p);
// ------------------------------------

// ----------- ultrasonic -------------
struct Proximity {
  bool initialised;
  long last_print;
  long last_trigger;
  uint8_t threshold;

  volatile bool update_raw;
  volatile uint8_t iteration_raw;
  volatile uint8_t reload_raw;
  uint16_t raw;
  uint16_t raw_prev;
  long last_raw;
};

static struct Proximity ultrasonic;

hw_timer_t *timer_10Hz_config = NULL;
// ------------------------------------

// ------------ sensor isr ------------
void IRAM_ATTR Timer_10Hz_ISR() { // every 0.1 seconds

  // refresh ultrasonic every 0.5 seconds
  if(ultrasonic.iteration_raw >= ultrasonic.reload_raw-1) {
    ultrasonic.update_raw = true;
    ultrasonic.iteration_raw = 0;
  } else {
    if(ultrasonic.update_raw == false) {
      ultrasonic.iteration_raw++;
    }
  }

}
// ------------------------------------


void setup() {
  Serial.begin(9600);
  
  print_wakeup_reason();

  initProximity();

  Serial << "Ready" << endl;
}


void loop() {
  
  updateProximity();

  commandLine();

}


void commandLine() {
  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'h':
        Serial << "h: help" << endl;
      break;
    }
  }
}


