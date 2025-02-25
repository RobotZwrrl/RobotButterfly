/*
 * Robot Butterfly IMU
 * -------------------------
 * Accelerometer and Gyroscope events from the
 * MPU6050 IMU
 * 
 * IMU library: https://github.com/ElectronicCats/mpu6050/
 * Moving Average library: https://github.com/JChristensen/movingAvg 
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
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg
#include "MPU6050.h"    // https://github.com/ElectronicCats/mpu6050/ 


// ------------ tests ------------
long last_print;
int counter = 0;
// -------------------------------


// ----------- imu -----------
hw_timer_t *timer2_cfg = NULL;
volatile bool new_avg_sample = false;

MPU6050 mpu;

enum IMUStates {
  IMU_SETTLE,
  IMU_CALIBRATE_HOME,
  IMU_ACTIVE
};

struct IMUData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  long last_data;
};

uint8_t IMU_STATE = IMU_ACTIVE;
static struct IMUData imu;
static struct IMUData imu_prev;
static struct IMUData imu_delta_avg;
static struct IMUData imu_home;

bool IMU_DATA_PRINT = false;
bool IMU_DATA_PRINT_RAW = false;
bool IMU_DATA_PRINT_DELTA_AVG = true;

movingAvg imu_avg_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_gz(IMU_MOVING_AVG_WINDOW);

long last_home_calibration = 0;
long last_imu_calib_print = 0;
long settle_start = 0;
// -----------------------------------


void IRAM_ATTR Timer2_ISR() {
  new_avg_sample = true;
}


// -------- imu callbacks ---------
// void buttonHoldNotificationCallback(uint8_t n);
// void buttonHoldReleasedCallback(uint8_t n);
// void buttonClickCallback(uint8_t n);
// void buttonReleaseCallback(uint8_t n);
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
      case 'e':
        IMU_STATE = IMU_SETTLE;
        settle_start = millis();
      break;
      case 'r':
        IMU_DATA_PRINT_RAW = !IMU_DATA_PRINT_RAW;
      break;
      case 'o':
        IMU_DATA_PRINT = !IMU_DATA_PRINT;
      break;
      case 'p':
        IMU_DATA_PRINT_DELTA_AVG = !IMU_DATA_PRINT_DELTA_AVG;
      break;
      case 'h':
        Serial << "e: IMU_STATE = IMU_SETTLE" << endl;
        Serial << "o: print imu data" << endl;
        Serial << "p: print imu delta avg" << endl;
        Serial << "r: print imu raw" << endl;
      break;
    }
  }

}


// ------------------------------------
// ---------- IMU Callbacks -----------
// ------------------------------------
/*
// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      tone(BUZZER_PIN, NOTE_F5, 500);
      noTone(BUZZER_PIN);
      delay(200);
    break;
    case BUTTON_LEFT:
      tone(BUZZER_PIN, NOTE_A5, 500);
      noTone(BUZZER_PIN);
    break;
    case BUTTON_RIGHT:
      tone(BUZZER_PIN, NOTE_A7, 500);
      noTone(BUZZER_PIN);
    break;
  }
}

// do an action here
void buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      counter *= 2;
    break;
    case BUTTON_LEFT:
      counter += 10;
    break;
    case BUTTON_RIGHT:
      counter -= 10;
    break;
  }
  Serial << "count: " << counter << endl;
}

// do an action here
void buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      tone(BUZZER_PIN, NOTE_A5, 100);
      noTone(BUZZER_PIN);
      counter++;
    break;
    case BUTTON_RIGHT:
      tone(BUZZER_PIN, NOTE_A7, 100);
      noTone(BUZZER_PIN);
      counter--;
    break;
  }
  Serial << "count: " << counter << endl;
}

// probably not necessary to do anything here
void buttonReleaseCallback(uint8_t n) {

}
*/

