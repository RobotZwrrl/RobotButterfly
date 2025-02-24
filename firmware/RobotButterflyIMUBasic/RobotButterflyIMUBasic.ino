/*
 * Robot Butterfly IMU Basic
 * -------------------------
 * Show that the IMU is working - both accelerometer
 * and gyroscope readings from the MPU6050
 * 
 * IMU library: https://github.com/ElectronicCats/mpu6050/
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 24 Feb 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include "MPU6050.h"    // https://github.com/ElectronicCats/mpu6050/ 


// ------------ tests ------------
long last_print;
int counter = 0;
// -------------------------------


// ----------- imu -----------
MPU6050 mpu;

struct IMUData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  long last_data;
};

static struct IMUData imu;

bool IMU_DATA_PRINT_RAW = true;
bool IMU_DATA_PRINT = false;
// -----------------------------------


void setup() {
  Serial.begin(9600);

  Wire.begin();

  print_wakeup_reason();

  initIMU();

  Serial << "Ready" << endl;
}


void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  updateIMU();

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'p':
        IMU_DATA_PRINT_RAW = !IMU_DATA_PRINT_RAW;
      break;
      case 'o':
        IMU_DATA_PRINT = !IMU_DATA_PRINT;
      break;
      case 'h':
        Serial << "p: print imu raw" << endl;
        Serial << "o: print imu data" << endl;
      break;
    }
  }

}

