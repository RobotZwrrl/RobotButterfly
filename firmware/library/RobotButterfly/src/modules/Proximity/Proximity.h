#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include <movingAvg.h>
#include <HCSR04.h>

// hi!
struct Proximity;

// ------------ callbacks ------------
typedef void (*ProximityCallback)(struct Proximity *p); // proximity callback type
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

  uint16_t val;
  uint16_t val_prev;
  movingAvg val_avg;

  ProximityCallback onProximityTriggerCallback;

  Proximity() 
  : initialised(false), last_print(0), last_trigger(0), threshold(0),
    update_raw(false), iteration_raw(0), reload_raw(0), raw(0), raw_prev(0), last_raw(0),
    val_avg(PROXIMITY_MOVING_AVG_WINDOW, true), onProximityTriggerCallback(NULL)
{}

};

extern struct Proximity ultrasonic;

static hw_timer_t *timer_10Hz_proximity_config;
// ------------------------------------

// ----------- functions -------------
void initProximity();
void updateProximity();
void Task_PROXIMITY_code(void * pvParameters);
void setProximityTaskPriority(uint8_t p);

uint16_t getRawProximityData(struct Proximity *p);
uint16_t getAvgProximityData(struct Proximity *p);

uint8_t getProximity8(struct Proximity *p, bool raw);
uint8_t getProximity10(struct Proximity *p, bool raw);
// ------------------------------------

// ---------- rtos ------------
static TaskHandle_t Task_PROXIMITY;
static SemaphoreHandle_t Mutex_PROXIMITY;
static long last_proximity_rtos_print;
// -------------------------------

#endif