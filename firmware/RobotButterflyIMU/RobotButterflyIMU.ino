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

enum IMUOrientations {
  IMU_TABLETOP,
  IMU_HANG,
  IMU_UNKNOWN
};

struct IMUData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  long last_data;
};

uint8_t IMU_STATE = IMU_ACTIVE;
uint8_t IMU_STATE_PREV = IMU_ACTIVE;

uint8_t IMU_ORIENTATION = IMU_UNKNOWN;
uint8_t IMU_ORIENTATION_PREV = IMU_UNKNOWN;
long last_orientation_change = 0;

static struct IMUData imu;
static struct IMUData imu_prev;
static struct IMUData imu_avg;
static struct IMUData imu_home;
static struct IMUData imu_delta_home_avg;
static struct IMUData imu_delta_time_avg;

bool IMU_PRINT_RAW = false;
bool IMU_PRINT_DATA_AVG = false;
bool IMU_PRINT_DELTA_HOME_AVG = true;
bool IMU_PRINT_DELTA_TIME_AVG = false;

movingAvg imu_avg_data_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gz(IMU_MOVING_AVG_WINDOW);

movingAvg imu_avg_home_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gz(IMU_MOVING_AVG_WINDOW);

movingAvg imu_avg_time_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_time_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_time_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_time_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_time_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_time_gz(IMU_MOVING_AVG_WINDOW);

long last_imu_calib_print = 0;
long last_home_calibration = 0;
long settle_start = 0;
long calibration_start = 0;
long last_score_clear = 0;
int home_recalibrate_score = 0;

// eg, left meaning it is the robot's left
enum IMUPoses {
  IMU_Pose_Tilt_L,
  IMU_Pose_Tilt_R,
  IMU_Pose_Tilt_Fwd,
  IMU_Pose_Tilt_Bwd,
  IMU_Pose_Home,
  IMU_Pose_NA
};

uint8_t IMU_POSE = IMU_Pose_NA;
long last_pose_detected = 0;
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
      case '1':
        Serial << "home position: " << endl;
        printIMUHome();
      break;
      case '2':
        Serial << "raw imu data: " << endl;
        printIMURaw();
      break;
      case 'e':
        IMU_STATE = IMU_SETTLE;
      break;
      case 'p':
        IMU_PRINT_DELTA_HOME_AVG = !IMU_PRINT_DELTA_HOME_AVG;
      break;
      case 'b':
        IMU_PRINT_RAW = !IMU_PRINT_RAW;
      break;
      case 'n':
        IMU_PRINT_DATA_AVG = !IMU_PRINT_DATA_AVG;
      break;
      case 'm':
        IMU_PRINT_DELTA_HOME_AVG = !IMU_PRINT_DELTA_HOME_AVG;
      break;
      case ',':
        IMU_PRINT_DELTA_TIME_AVG = !IMU_PRINT_DELTA_TIME_AVG;
      break;
      case 'h':
        Serial << "h: help" << endl;
        Serial << "1: print imu home frame" << endl;
        Serial << "2: print imu raw data frame" << endl;
        Serial << "e: IMU_STATE = IMU_SETTLE" << endl;

        if(IMU_PRINT_DELTA_HOME_AVG) Serial << "*";
        Serial << "p: IMU_PRINT_DELTA_HOME_AVG" << endl;
        if(IMU_PRINT_RAW) Serial << "*";
        Serial << "b: IMU_PRINT_RAW" << endl;
        if(IMU_PRINT_DATA_AVG) Serial << "*";
        Serial << "n: IMU_PRINT_DATA_AVG" << endl;
        if(IMU_PRINT_DELTA_HOME_AVG) Serial << "*";
        Serial << "m: IMU_PRINT_DELTA_HOME_AVG" << endl;
        if(IMU_PRINT_DELTA_TIME_AVG) Serial << "*";
        Serial << ",: IMU_PRINT_DELTA_TIME_AVG" << endl;
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

