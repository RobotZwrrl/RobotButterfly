#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include <movingAvg.h>
#include "DHT.h"

// hi!
struct Sensor;


// ------------ callbacks ------------
typedef void (*SensorTriggerCallback)(struct Sensor *s, bool trigger_dir);
typedef void (*SensorAmbientCallback)(struct Sensor *s, int change);
// ------------------------------------

// -------------- vars ---------------
extern DHT dht;

uint16_t getSensor_Light(struct Sensor *s);
void updateSensor_Light(struct Sensor *s);
uint16_t getSensor_Battery(struct Sensor *s);
void updateSensor_Battery(struct Sensor *s);
uint16_t getSensor_Sound(struct Sensor *s);
void updateSensor_Sound(struct Sensor *s);
uint16_t getSensor_Temperature(struct Sensor *s);
void updateSensor_Temperature(struct Sensor *s);
uint16_t getSensor_Humidity(struct Sensor *s);
void updateSensor_Humidity(struct Sensor *s);
typedef uint16_t (*SensorDAQFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
typedef void (*SensorUpdateFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
// ------------------------------------

// ------------- sensor ---------------
static hw_timer_t *timer_10Hz_config;

enum SensorIDs {
  SENSOR_ID_LIGHT,  
  SENSOR_ID_BATTERY,
  SENSOR_ID_SOUND,
  SENSOR_ID_TEMPERATURE,
  SENSOR_ID_HUMIDITY
};

struct Sensor {
  uint8_t id;
  String name;
  bool print;
  long print_frequency;
  long last_print;
  uint16_t trig_count;
  bool trigger_dir;
  long last_sensor_trigger;
  long last_ambient_trigger;

  // collected every 0.1 seconds
  volatile bool update_raw;
  volatile uint8_t iteration_raw;
  volatile uint8_t reload_raw;
  uint16_t raw;
  uint16_t raw_prev;
  long last_raw;

  // averaged over 1 seconds
  volatile bool update_val;
  volatile uint8_t iteration_val;
  volatile uint8_t reload_val;
  uint16_t val;
  uint16_t val_prev;
  long last_val;
  movingAvg val_avg;

  // averaged over 60 seconds
  volatile bool update_ambient;
  volatile uint16_t iteration_ambient;
  volatile uint16_t reload_ambient;
  uint16_t ambient;
  uint16_t ambient_prev;
  long last_ambient;
  movingAvg ambient_avg;
  int ambient_data[6];

  // ---
  SensorDAQFunction getRawData;        // function pointer
  SensorUpdateFunction updateSensor;   // function pointer

  SensorTriggerCallback onSensorChangeCallback;
  SensorAmbientCallback onSensorAmbientChangeCallback;

  // constructor
  Sensor()
  : id(0), name(""), print(true), print_frequency(0), last_print(0), trig_count(0), trigger_dir(false), last_sensor_trigger(0), last_ambient_trigger(0),
    update_raw(false), iteration_raw(0), reload_raw(0), raw(0), raw_prev(0), last_raw(0),
    update_val(false), iteration_val(0), reload_val(0), val(0), val_prev(0), last_val(0),
    val_avg(SENSOR_MOVING_AVG_VAL_WINDOW, true),
    update_ambient(false), iteration_ambient(0), reload_ambient(0), ambient(0), ambient_prev(0), last_ambient(0),
    ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW, true),
    ambient_data(),
    getRawData(NULL), updateSensor(NULL),
    onSensorChangeCallback(NULL), onSensorAmbientChangeCallback(NULL)
  {}

};

extern struct Sensor sensor_temperature;
extern struct Sensor sensor_humidity;
extern struct Sensor sensor_light;
extern struct Sensor sensor_sound;
extern struct Sensor sensor_battery;

extern struct Sensor *all_sensors[NUM_SENSORS];

static bool dht_toggle;
static bool dht_processed;
static long last_dht_processed;
// ------------------------------------

// ------------ functions -------------
void initSensors();
void updateSensors();
void printSensor(struct Sensor *s);
void Task_SENSORS_code(void * pvParameters);
void setSensorsTaskPriority(uint8_t p);

uint16_t getSensor_Battery(struct Sensor *s);
void updateSensor_Battery(struct Sensor *s);
void initSensor_Battery(struct Sensor *s);

uint16_t getSensor_Humidity(struct Sensor *s);
void updateSensor_Humidity(struct Sensor *s);
void initSensor_Humidity(struct Sensor *s);

uint16_t getSensor_Light(struct Sensor *s);
void updateSensor_Light(struct Sensor *s);
void initSensor_Light(struct Sensor *s);

uint16_t getSensor_Sound(struct Sensor *s);
void updateSensor_Sound(struct Sensor *s);
void initSensor_Sound(struct Sensor *s);

uint16_t getSensor_Temperature(struct Sensor *s);
void updateSensor_Temperature(struct Sensor *s);
void initSensor_Temperature(struct Sensor *s);
// ------------------------------------

// ---------- rtos ------------
static TaskHandle_t Task_SENSORS;
static SemaphoreHandle_t Mutex_SENSORS;
static long last_sensors_rtos_print;
// -------------------------------

#endif