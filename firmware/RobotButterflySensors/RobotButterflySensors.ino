/*
 * Robot Butterfly Sensors
 * -------------------------
 * Gather data from the temperature, humidity,
 * light, sound, and battery sensors
 * 
 * Timer 2 fires every 0.1 seconds (shared with IMU)
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
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg


// ------------ callbacks ------------
// TODO
// ------------------------------------

// --------------- imu ----------------
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
  bool print;
  
  // collected every 0.1 seconds
  bool update_raw;
  uint8_t iteration_raw;
  uint8_t reload_raw;
  uint16_t raw;
  uint16_t raw_prev;
  long last_raw;

  // averaged over 1 seconds
  bool update_val;
  uint8_t iteration_val;
  uint8_t reload_val;
  uint16_t val;
  uint16_t val_prev;
  long last_val;

  // averaged over 5 seconds
  bool update_ambient;
  uint8_t iteration_ambient;
  uint8_t reload_ambient;
  uint16_t ambient;
  long last_ambient;
  int ambient_vals[NUM_AMBIENT_VALS];
};

volatile static struct Sensor sensor_temperature;
volatile static struct Sensor sensor_humidity;
volatile static struct Sensor sensor_light;
volatile static struct Sensor sensor_sound;
volatile static struct Sensor sensor_battery;

volatile static struct Sensor *all_sensors[NUM_SENSORS];

movingAvg sensor_light_raw(SENSOR_MOVING_AVG_RAW_WINDOW);
movingAvg sensor_light_ambient(SENSOR_MOVING_AVG_AMBIENT_WINDOW);
// ------------------------------------

// ------------- imu isr --------------
void IRAM_ATTR Timer_10Hz_ISR() { // every 0.1 seconds
  
  // get the raw data every 0.1 second
  if(sensor_light.iteration_raw >= sensor_light.reload_raw-1) {
    sensor_light.update_raw = true; 
    sensor_light.iteration_raw = 0;
  } else {
    sensor_light.update_raw = false;
    sensor_light.iteration_raw++;
  }

  // update the val every 1 second
  if(sensor_light.iteration_val >= sensor_light.reload_val-1) {
    sensor_light.update_val = true;
    sensor_light.iteration_val = 0;
  } else {
    sensor_light.update_val = false;
    sensor_light.iteration_val++;
  }

  // update ambient every 5 seconds
  if(sensor_light.iteration_ambient >= sensor_light.reload_ambient-1) {
    sensor_light.update_ambient = true;
    sensor_light.iteration_ambient = 0;
  } else {
    sensor_light.update_ambient = false;
    sensor_light.iteration_ambient++;
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



