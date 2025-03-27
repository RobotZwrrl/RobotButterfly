/*
 * Robot Butterfly Sensors
 * -------------------------
 * Gather data from the sensors: temperature, humidity,
 * light, sound, and battery
 * 
 * Timer 2 fires every 0.1 seconds
 * 
 * IMU library: https://github.com/ElectronicCats/mpu6050/
 * Moving Average library: https://github.com/JChristensen/movingAvg 
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 22 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <movingAvg.h>   // 2.3.2 https://github.com/JChristensen/movingAvg
#include "DHT.h"         // 1.4.6 https://github.com/adafruit/DHT-sensor-library & 1.1.15 https://github.com/adafruit/Adafruit_Sensor 


// ------------ callbacks ------------
void sensorLightChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorLightAmbientChangeCallback(struct Sensor *s, int change);
void sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change);
// ------------------------------------

// -------------- vars ---------------
DHT dht(DHT11_PIN, DHT11);

long last_print = 0;

uint16_t getSensor_Light(struct Sensor *s);
void updateSensor_Light(struct Sensor *s);
uint16_t getSensor_Battery(struct Sensor *s);
void updateSensor_Battery(struct Sensor *s);
uint16_t getSensor_Sound(struct Sensor *s);
void updateSensor_Sound(struct Sensor *s);
uint16_t getSensor_Temperature(struct Sensor *s);
void updateSensor_Temperature(struct Sensor *s);
typedef uint16_t (*SensorDAQFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
typedef void (*SensorUpdateFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
// ------------------------------------

// --------------- sensor ----------------
hw_timer_t *timer_10Hz_config = NULL;
volatile bool new_avg_sample = false;

enum SensorIDs {
  SENSOR_ID_TEMPERATURE,
  SENSOR_ID_HUMIDITY,
  SENSOR_ID_LIGHT,
  SENSOR_ID_SOUND,
  SENSOR_ID_BATTERY
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

  // constructor
  Sensor()
  : id(0), name(""), print(true), print_frequency(0), last_print(0), trig_count(0), trigger_dir(false), last_sensor_trigger(0), last_ambient_trigger(0),
    update_raw(false), iteration_raw(0), reload_raw(0), raw(0), raw_prev(0), last_raw(0),
    update_val(false), iteration_val(0), reload_val(0), val(0), val_prev(0), last_val(0),
    val_avg(SENSOR_MOVING_AVG_VAL_WINDOW, true),
    update_ambient(false), iteration_ambient(0), reload_ambient(0), ambient(0), ambient_prev(0), last_ambient(0),
    ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW, true),
    ambient_data(),
    getRawData(NULL), updateSensor(NULL)
  {}

};

static struct Sensor sensor_temperature;
static struct Sensor sensor_humidity;
static struct Sensor sensor_light;
static struct Sensor sensor_sound;
static struct Sensor sensor_battery;

static struct Sensor *all_sensors[NUM_SENSORS];
// ------------------------------------

// ------------- sensor isr --------------
void IRAM_ATTR Timer_10Hz_ISR() { // every 0.1 seconds

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;

    // update raw every 0.1 second
    if(s->iteration_raw >= s->reload_raw-1) {
      s->update_raw = true; 
      s->iteration_raw = 0;
    } else {
      if(s->update_raw == false) {
        s->iteration_raw++;
      }
    }

    // update val every 1 second
    if(s->iteration_val >= s->reload_val-1) {
      s->update_val = true;
      s->iteration_val = 0;
    } else {
      if(s->update_val == false) {
        s->iteration_val++;
      }
    }

    // update ambient every 60 seconds
    if(s->iteration_ambient >= s->reload_ambient-1) {
      s->update_ambient = true;
      s->iteration_ambient = 0;
    } else {
      if(s->update_ambient == false) {
        s->iteration_ambient++;
      }
    }

  }

}
// ------------------------------------

void setup() {
  Serial.begin(9600);
  
  print_wakeup_reason();

  initSensors();

  Serial << "Ready" << endl;
}


void loop() {
  
  //friendlyPrint();

  updateSensors();

  console();

}


void console() {
  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'h':
        Serial << "h: help" << endl;
      break;
    }
  }
}



