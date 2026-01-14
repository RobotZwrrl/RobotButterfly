#include "modules/Proximity/Proximity.h"

Proximity ultrasonic;

// ---------- proximity isr ----------
void IRAM_ATTR Timer_10Hz_Proximity_ISR() { // every 0.1 seconds

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

void updateProximity() {

  // entrance checks
  if(!ultrasonic.initialised) return;
  if(millis() < PROXIMITY_WARMUP_TIME) return; // let sensor warm up
  if(!ultrasonic.update_raw) return; // flag set by isr

  // read the sensor and clamp the values
  double *distances = HCSR04.measureDistanceCm();
  int dist = (int)floor(distances[0]);
  if(dist > PROXIMITY_MAX) dist = PROXIMITY_MAX;
  if(dist < PROXIMITY_MIN) dist = PROXIMITY_MIN;
  
  // update raw
  ultrasonic.raw_prev = ultrasonic.raw;
  ultrasonic.raw = dist;

  // moving average filter update
  ultrasonic.val_prev = ultrasonic.val;
  ultrasonic.val_avg.reading(ultrasonic.raw);
  ultrasonic.val = ultrasonic.val_avg.getAvg();

  // detect close proximity trigger
  if(ultrasonic.raw <= ultrasonic.threshold && millis()-ultrasonic.last_trigger > PROXIMITY_TRIGGER_FREQ) {
    if(ultrasonic.onProximityTriggerCallback) ultrasonic.onProximityTriggerCallback(&ultrasonic);
    ultrasonic.last_trigger = millis();
  }

  // print
  if(millis()-ultrasonic.last_print >= 100) {
    if(DEBUG_PROXIMITY) Serial << "Proximity raw: " << ultrasonic.raw << " avg: " << ultrasonic.val << endl;
    ultrasonic.last_print = millis();
  }

  // reset the flag for the isr
  noInterrupts();
    ultrasonic.update_raw = false;
  interrupts();

}


void initProximity() {

  HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);
  ultrasonic.initialised = true;

  ultrasonic.threshold = PROXIMITY_THRESHOLD;
  ultrasonic.last_trigger = 0;
  ultrasonic.last_print = 0;
  ultrasonic.iteration_raw = 0;
  ultrasonic.update_raw = false;
  ultrasonic.reload_raw = 1*5;        // every 0.5 seconds
  ultrasonic.val_avg.begin();

  // set up timer 3 for every 0.1 second
  // params: timer #3, prescaler amount, count up (true)
  timer_10Hz_proximity_config = timerBegin(3, 16000, true);
  timerAttachInterrupt(timer_10Hz_proximity_config, &Timer_10Hz_Proximity_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_proximity_config, 500, true);
  timerAlarmEnable(timer_10Hz_proximity_config);

  if(RTOS_ENABLED) {

    Mutex_PROXIMITY = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_PROXIMITY_code,     // task function
                      "Task_PROXIMITY",        // name of task
                      STACK_PROXIMITY,         // stack size of task
                      NULL,                    // parameter of the task
                      PRIORITY_PROXIMITY_MID,  // priority of the task (low number = low priority)
                      &Task_PROXIMITY,         // task handle to keep track of created task
                      TASK_CORE_PROXIMITY);    // pin task to core

  }

}


// collected every 0.1 seconds
uint16_t getRawProximityData(struct Proximity *p) {
  return p->raw;
}


// averaged over 1 second (from the raw values)
uint16_t getAvgProximityData(struct Proximity *p) {
  return p->val;
}


// from 0-7 inclusive for feeding neopixel strip
// based on the moving average value
uint8_t getProximity8(struct Proximity *p, bool raw) {
  if(raw) {
    return map(p->raw, PROXIMITY_MIN, PROXIMITY_MAX, 0, 7);
  } else {
    return map(p->val, PROXIMITY_MIN, PROXIMITY_MAX, 0, 7);
  }
}

// from 0-9 inclusive for feeding servo range animation
// based on the moving average value
uint8_t getProximity10(struct Proximity *p, bool raw) {
  if(raw) {
    return map(p->raw, PROXIMITY_MIN, PROXIMITY_MAX, 0, 9);
  } else {
    return map(p->val, PROXIMITY_MIN, PROXIMITY_MAX, 0, 9);
  }
}


void Task_PROXIMITY_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_PROXIMITY, (TickType_t)10) == pdTRUE) {
      
      updateProximity();

      if(DEBUG_PROXIMITY_RTOS == true && millis()-last_proximity_rtos_print >= 1000) {
        Serial << "proximity stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
        last_proximity_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_PROXIMITY);
    }
    
    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_PROXIMITY != 0) vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_PROXIMITY );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setProximityTaskPriority(uint8_t p) {
  
  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_PROXIMITY);
  vTaskPrioritySet(Task_PROXIMITY, p);
  if (DEBUG_PROXIMITY_RTOS) Serial << "changed PROXIMITY task priority - new: " << p << " prev: " << prev_priority << endl;

}

