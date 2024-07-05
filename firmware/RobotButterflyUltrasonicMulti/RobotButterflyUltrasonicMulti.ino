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
#include "esp_task_wdt.h" // this is included to feed the dog
#include "soc/timer_group_struct.h" // this is included to write to the wdt registers directly
#include "soc/timer_group_reg.h" // this is included to write to the wdt registers directly

TaskHandle_t Task_US;
QueueHandle_t Queue_US1;
QueueHandle_t Queue_US3;
QueueHandle_t Queue_US2;

hw_timer_t *timer0_cfg = NULL;
volatile bool updateUS = false;
volatile bool restartUS = false;
volatile uint32_t restartUS_count = 0;
uint32_t valuesUS_count = 0;
long last_print;

struct RawDataPoint_US {
  double dist_cm_raw;
  int dist_8_raw;
  long record_time;
};
struct RawDataPoint_US raw_us;

void IRAM_ATTR Timer0_ISR() {
  updateUS = true;

  // check that the sensor is still sending data recently
  if(millis()-raw_us.record_time > CHECK_FREQ_US && 
     millis() > STARTUP_WAIT_US && 
     restartUS == false) {
    restartUS = true;
    restartUS_count++;
  }

}


void setup() {
  Serial.begin(9600);

  HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  // ultrasonic data digester
  // set up timer 0 for every 0.5 second
  timer0_cfg = timerBegin(16000);
  timerAttachInterrupt(timer0_cfg, &Timer0_ISR);
  // params: timer, tick count, auto-reload (true), reload count (0 = infinite)
  timerAlarm(timer0_cfg, 2500, true, 0);
  timerStart(timer0_cfg);

  initUltrasonic();

}


void initUltrasonic() {

  if(Queue_US1 == NULL && Queue_US2 == NULL && Queue_US3 == NULL) {
    // create the queue with 1 slots of 4 bytes
    // more slots = more numbers
    Queue_US1 = xQueueCreate(1, sizeof(double));
    Queue_US2 = xQueueCreate(1, sizeof(int));
    Queue_US3 = xQueueCreate(1, sizeof(long));
  }

  //disableCore0WDT(); // can do this if we want

  // core 0 has task watchdog enabled to protect wifi service etc
  // core 1 does not have watchdog enabled
  xTaskCreatePinnedToCore(
                    Task_US_code,  // task function
                    "Task_US",     // name of task
                    5000,          // stack size of task
                    NULL,          // parameter of the task
                    1,             // priority of the task
                    &Task_US,      // task handle to keep track of created task
                    0);            // pin task to core 0

}


void Task_US_code(void * pvParameters) {

  if(DEBUG_US) Serial << "Task_US_code" << endl;
  
  while(1) {

    if(DEBUG_US) Serial << "Reading HC-SR04" << endl;

    if(millis() < STARTUP_WAIT_US) continue; // let sensor warm up

    double *distances = HCSR04.measureDistanceCm();
    double dist_cm = distances[0];
    
    if(dist_cm < 0) { // error
      Serial << endl;
      Serial << millis() << " [" << xPortGetCoreID() << "] ";
      Serial << "ERROR US" << endl;
      Serial << "---------" << endl;
      vTaskDelay(1);
      continue;
    }

    int dist = (int)floor(dist_cm);
    if(dist > DIST_MAX) dist = DIST_MAX;
    if(dist < DIST_MIN) dist = DIST_MIN;
    int neo_map = (int)map(dist, DIST_MIN, DIST_MAX, 1, 8);

    if(DEBUG_US) Serial << "dist_cm: " << dist_cm << " dist: " << dist << " neo_map: " << neo_map << endl;

    long last_sample = millis();

    if(DEBUG_US) Serial << millis() << " [" << xPortGetCoreID() << "] ";
    if(DEBUG_US) Serial << "Adding to queue US " << dist_cm << ", " << neo_map << ", " << last_sample << endl;

    // add to the queue
    // block time of 0 says don't block if the queue is already full
    xQueueSend(Queue_US1, &dist_cm, (TickType_t)0);
    xQueueSend(Queue_US2, &neo_map, (TickType_t)0);
    xQueueSend(Queue_US3, &last_sample, (TickType_t)0);

    if(DEBUG_US) Serial << "task delay now" << endl;
    vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil( &xLastWakeTime, SAMPLE_FREQ_US );
  }

  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void loop() {
  
  if(millis()-last_print >= SAMPLE_FREQ_US) {
    Serial << millis() << " hi " << xPortGetCoreID();
    Serial << " dist_cm_raw = " << raw_us.dist_cm_raw;
    Serial << " dist_8_raw = " << raw_us.dist_8_raw;
    Serial << endl;
    last_print = millis();
  }

  updateUltrasonic();

  // for testing purposes
  if(Serial.available()) {
    char c = Serial.read();
    if(c == 'r') {
      restartUS = true;
      Serial << "\r\n\r\n\r\n" << endl;
    }
  }

}



// /** Task states returned by eTaskGetState. */
// typedef enum
// {
//     eRunning = 0,     /* A task is querying the state of itself, so must be running. */
//     eReady,           /* The task being queried is in a read or pending ready list. */
//     eBlocked,         /* The task being queried is in the Blocked state. */
//     eSuspended,       /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
//     eDeleted,         /* The task being queried has been deleted, but its TCB has not yet been freed. */
//     eInvalid          /* Used as an 'invalid state' value. */
// } eTaskState;

void updateUltrasonic() {

  if(restartUS) {

    if(Task_US == NULL) {
      Serial << "Restarting Task_US" << endl;
      initUltrasonic();
      Serial << "\r\n\r\n\r\n" << endl;
      restartUS = false;
      return;
    }

    eTaskState ts = eTaskGetState(Task_US);
    Serial << millis() << " Task_US state: " << ts << endl;
    if(ts == eDeleted) { // deleted
      Serial << "Restarting Task_US" << endl;
      initUltrasonic();
      Serial << "\r\n\r\n\r\n" << endl;
      restartUS = false;
      return;
    } else if(ts == eBlocked || ts == eSuspended) { // suspended or blocked
      // 8681 Task_US state: 2
      // Deleting Task_US
      // ^ crash
      Serial << "Deleting Task_US" << endl;
      vTaskDelete(Task_US);
      Task_US == NULL;
      restartUS = false;
      return;
    // } else if(ts == 2) { // blocked
    //   // not sure what to do here
    //   // 27729 Task_US state: 2
    //   // Suspending Task_US
    //   // ^ this is when it crash
    //   // maybe try doing nothing and see what happens?
    //   // could be a good time to feed the watchdog
    //   Serial << "Doing nothing Task_US" << endl;
    //   restartUS = false;
    } else { // other
      Serial << "Suspending Task_US" << endl;
      //102524 Task_US state: 0
      //Suspending Task_US
      // ^ crash
      // 9321 Task_US state: 1
      // Suspending Task_US
      // ^ crash
      vTaskSuspend(Task_US);
    }

    /*
    Serial << "Restarting Task_US " << restartUS_count << endl;
    
    // notes:
    // this is the correct way to feed the dog
    // this method will not work if there are any other threads,
    // because it checks to make sure all threads have called 
    // esp_task_wdt_reset() recently, and only feeds the dog 
    // if so
    // via: https://forum.arduino.cc/t/esp32-a-better-way-than-vtaskdelay-to-get-around-watchdog-crash/596889/13
    // esp_task_wdt_reset();

    eTaskState ts = eTaskGetState(Task_US);
    Serial << "state: " << ts << endl;

    // check if the task was able to delete itself
    if(ts < 4) { 
      Serial << "step A" << endl;
      xTaskAbortDelay(Task_US);

      ts = eTaskGetState(Task_US);
      Serial << "state: " << ts << endl;
      Serial << "step B1" << endl;
      vTaskSuspend(Task_US);

      ts = eTaskGetState(Task_US);
      Serial << "state: " << ts << endl;
      Serial << "step B2" << endl;
      vTaskDelete(Task_US);

      ts = eTaskGetState(Task_US);
      Serial << "state: " << ts << endl;
    }

    ts = eTaskGetState(Task_US);
    Serial << "state: " << ts << endl;
    Serial << "step C" << endl;
    initUltrasonic();

    ts = eTaskGetState(Task_US);
    Serial << "state: " << ts << endl;
    Serial << "step D" << endl;

    restartUS = false;
    */
  }

  //if(!updateUS) return; // not needed, this updates continuously

  uint8_t ready = 0;
  double a;
  int b;
  long c;

  // check the queues
  if(uxQueueMessagesWaiting(Queue_US1) > 0) ready++;
  if(uxQueueMessagesWaiting(Queue_US2) > 0) ready++;
  if(uxQueueMessagesWaiting(Queue_US3) > 0) ready++;
  
  if(ready < 3) {
    //Serial << "Queue messages not ready" << endl;
    return;
  }

  // read the queues
  xQueueReceive(Queue_US1, &a, 0);
  xQueueReceive(Queue_US2, &b, 0);
  xQueueReceive(Queue_US3, &c, 0);

  raw_us.dist_cm_raw = a;
  raw_us.dist_8_raw = b;
  raw_us.record_time = c;
  valuesUS_count++;

  if(DEBUG_US) Serial << millis() << " Received from queue US (" << valuesUS_count << "): " << raw_us.dist_cm_raw << ", " << raw_us.dist_8_raw << ", " << raw_us.record_time << endl;
  
  // flush the queues
  xQueueReset(Queue_US1);
  xQueueReset(Queue_US2);
  xQueueReset(Queue_US3);

  // reset flag
  updateUS = false;

}


// source: https://forum.arduino.cc/t/esp32-a-better-way-than-vtaskdelay-to-get-around-watchdog-crash/596889/13
void feedTheDog() {
  // feed dog 0
  // TIMERG0.wdtwprotect=TIMG_WDT_WKEY_VALUE; // write enable

  // const timg_wdtfeed_reg_t wdtfeed_val;
  // wdtfeed_val.val = 1;

  // TIMERG0.wdtfeed=wdtfeed_val;    // feed dog
  //TIMERG0.wdtwprotect=(timg_wdtwprotect_reg_t)0;                   // write protect
  // // feed dog 1
  // TIMERG1.wdtwprotect=TIMG_WDT_WKEY_VALUE; // write enable
  // TIMERG1.wdtfeed=1;                       // feed dog
  // TIMERG1.wdtwprotect=0;                   // write protect
}

