/*
 * Robot Butterfly Temperature & Humidity Multitask
 * -------------------------------------------------
 * Sample the DHT11 sensor in a separate task, transfer
 * the data via a queue. Restart the task if needed.
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 *
 * 27 Jun 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
// dht11 lib requires modifying TIMEOUT_DURATION = 0; in DHT11.h
#include <DHT11.h>      // DHT11 library v2.1.0 by Dhruba Saha https://github.com/dhrubasaha08/DHT11 


TaskHandle_t Task_TH;
QueueHandle_t Queue_TH1;
QueueHandle_t Queue_TH2;
QueueHandle_t Queue_TH3;

DHT11 dht11(DHT11_PIN);

hw_timer_t *timer0_cfg = NULL;
volatile bool updateTH = false;
volatile bool restartTH = false;
volatile uint32_t restartTH_count = 0;
uint32_t valuesTH_count = 0;

struct RawDataPoint_TH {
  int temp_raw;
  int humid_raw;
  long record_time;
};
struct RawDataPoint_TH raw_data_th;

void IRAM_ATTR Timer0_ISR() {
  updateTH = true;

  // check that the sensor is still sending data recently
  if(millis()-raw_data_th.record_time > 1500 && millis() > STARTUP_WAIT_TH && restartTH == false) {//SAMPLE_FREQ_TH) {
    restartTH = true;
    restartTH_count++;
  }

}


long last_print;


void setup() {
  Serial.begin(9600);

  dht11.setDelay(0); // their library has a blocking delay, set to 0

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  // temphumid data digester
  // set up timer 0 for every 0.5 second
  // params: timer #0, prescaler amount, count up (true)
  timer0_cfg = timerBegin(0, 16000, true);
  timerAttachInterrupt(timer0_cfg, &Timer0_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer0_cfg, 2500, true);
  timerAlarmEnable(timer0_cfg);

  initTempHumid();

}


void initTempHumid() {

  if(Queue_TH1 == NULL && Queue_TH2 == NULL && Queue_TH3 == NULL) {
    // create the queue with 1 slots of 4 bytes
    // more slots = more numbers
    Queue_TH1 = xQueueCreate(1, sizeof(int));
    Queue_TH2 = xQueueCreate(1, sizeof(int));
    Queue_TH3 = xQueueCreate(1, sizeof(long));
  }

  // core 0 has task watchdog enabled to protect wifi service etc
  // core 1 does not have watchdog enabled
  xTaskCreatePinnedToCore(
                    Task_TH_code,  // task function
                    "Task_TH",     // name of task
                    1024,          // stack size of task
                    NULL,          // parameter of the task
                    1,             // priority of the task
                    &Task_TH,      // task handle to keep track of created task
                    0);            // pin task to core 0

}


void Task_TH_code(void * pvParameters) {

  if(DEBUG_TH) Serial << "Task_TH_code" << endl;

  int temp_raw = 0;
  int humid_raw = 0;
  long last_sample = 0;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while(1) {

    if(DEBUG_TH) Serial << "Reading DHT11" << endl;

    if(millis() < STARTUP_WAIT_TH) continue; // doesn't work on startup

    if(dht11.readTemperatureHumidity(temp_raw, humid_raw) != 0) {
      Serial << millis() << " [" << xPortGetCoreID() << "] ";
      Serial << "ERROR" << endl;
      Serial << "---------" << endl;
      continue;
    }

    if(temp_raw == 0 && humid_raw == 0) { // bad reading
      Serial << millis() << " [" << xPortGetCoreID() << "] ";
      Serial << "BAD READING" << endl;
      Serial << "---------" << endl;
      continue;
    }

    last_sample = millis();

    if(DEBUG_TH) Serial << millis() << " [" << xPortGetCoreID() << "] ";
    if(DEBUG_TH) Serial << "Adding to queue " << temp_raw << ", " << humid_raw << ", " << last_sample << endl;

    // add to the queue
    // block time of 0 says don't block if the queue is already full
    xQueueSend(Queue_TH1, &temp_raw, (TickType_t)0);
    xQueueSend(Queue_TH2, &humid_raw, (TickType_t)0);
    xQueueSend(Queue_TH3, &last_sample, (TickType_t)0);

    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil( &xLastWakeTime, SAMPLE_FREQ_TH );

  }

  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void loop() {
  
  if(millis()-last_print >= 1000) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

  updateTempHumid();

}


void updateTempHumid() {

  if(restartTH) {
    Serial << "Restarting Task_TH " << restartTH_count << endl;
    vTaskSuspend(Task_TH);
    vTaskDelete(Task_TH);
    delay(50); // necessary delay it seems, 10 ms is too short
    initTempHumid();
    restartTH = false;
  }

  if(!updateTH) return;

  uint8_t messages_ready_count = 0;
  int a;
  int b;
  long c;

  // check the queues
  if(uxQueueMessagesWaiting(Queue_TH1) > 0) messages_ready_count++;
  if(uxQueueMessagesWaiting(Queue_TH2) > 0) messages_ready_count++;
  if(uxQueueMessagesWaiting(Queue_TH3) > 0) messages_ready_count++;
  
  if(messages_ready_count < 3) {
    //Serial << "Queue messages not ready" << endl;
    return;
  }

  // read the queues
  xQueueReceive(Queue_TH1, &a, 0);
  xQueueReceive(Queue_TH2, &b, 0);
  xQueueReceive(Queue_TH3, &c, 0);

  raw_data_th.temp_raw = a;
  raw_data_th.humid_raw = b;
  raw_data_th.record_time = c;
  valuesTH_count++;

  Serial << millis() << " Received from queue (" << valuesTH_count << "): " << raw_data_th.temp_raw << ", " << raw_data_th.humid_raw << ", " << raw_data_th.record_time << endl;
  
  // flush the queues
  xQueueReset(Queue_TH1);
  xQueueReset(Queue_TH2);
  xQueueReset(Queue_TH3);

  // reset flag
  updateTH = false;

}


