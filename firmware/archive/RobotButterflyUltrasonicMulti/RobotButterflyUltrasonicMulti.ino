/*
 * Robot Butterfly Ultrasonic Sensor Multitask
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
 * 30 Jun 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <HCSR04.h>     // HCSR04 library v1.1.2 by Dirk Sarodnick https://github.com/d03n3rfr1tz3/HC-SR04 


hw_timer_t *timer0_cfg = NULL;
long last_print;

// ----------- ultrasonic -----------
TaskHandle_t Task_US;
QueueHandle_t Queue_US1;
QueueHandle_t Queue_US3;
QueueHandle_t Queue_US2;

volatile bool updateUS = false;
uint32_t valuesUS_count = 0;
long last_print_us = 0;
bool initialised_sensor = false;

struct RawDataPoint_US {
  double dist_cm_raw;
  int dist_8_raw;
  long record_time;
};
struct RawDataPoint_US raw_us;
// ----------------------------------

void IRAM_ATTR Timer0_ISR() {
  updateUS = true;
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

  initUltrasonic();

}


void loop() {
  
  if(millis()-last_print >= 500) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

  updateUltrasonic();

}

