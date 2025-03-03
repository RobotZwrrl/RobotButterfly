/*
 * Robot Butterfly IMU
 * -------------------------
 * Accelerometer and Gyroscope events from the
 * MPU6050 IMU
 * 
 * 'Left' refers to the robot's left
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


// ------------ callbacks ------------
void imuStateChangeCallback(uint8_t s);
void imuOrientationChangeCallback(uint8_t o);
void imuPoseChangeCallback(uint8_t p);
void imuEventDetectedCallback(uint8_t e);
// ------------------------------------

// --------------- imu ----------------
hw_timer_t *timer2_cfg = NULL;
volatile bool new_avg_sample = false;

MPU6050 mpu;

struct IMUData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  long last_data;
};

static struct IMUData imu;
static struct IMUData imu_prev;
static struct IMUData imu_avg;
static struct IMUData imu_home;
static struct IMUData imu_delta_home_avg;
static struct IMUData imu_delta_time_avg;

enum IMUStates {
  IMU_SETTLE,
  IMU_CALIBRATE_HOME,
  IMU_ACTIVE,
  IMU_INACTIVE
};

uint8_t IMU_STATE = IMU_INACTIVE;
uint8_t IMU_STATE_PREV = IMU_INACTIVE;

enum IMUOrientations {
  IMU_TABLETOP,
  IMU_HANG,
  IMU_UNKNOWN
};

uint8_t IMU_ORIENTATION = IMU_UNKNOWN;
uint8_t IMU_ORIENTATION_PREV = IMU_UNKNOWN;
long last_orientation_change = 0;

enum IMUPoses {
  IMU_Pose_Tilt_L,
  IMU_Pose_Tilt_R,
  IMU_Pose_Tilt_Fwd,
  IMU_Pose_Tilt_Bwd,
  IMU_Pose_Home,
  IMU_Pose_NA
};

uint8_t IMU_POSE = IMU_Pose_NA;
uint8_t IMU_POSE_PREV = IMU_Pose_NA;
long last_pose_detected = 0;

bool EVENT_DETECTED = false;
long last_event_detected = 0;
long last_event_score_clear = 0;
int event_score = 0;

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

bool IMU_PRINT_RAW = false;
bool IMU_PRINT_DATA_AVG = false;
bool IMU_PRINT_DELTA_HOME_AVG = false; // usually true during dev
bool IMU_PRINT_DELTA_TIME_AVG = false;
bool IMU_PRINT_STATS = true; // usually true during testing
// ------------------------------------

// ------------- imu isr --------------
void IRAM_ATTR Timer2_ISR() {
  new_avg_sample = true;
}
// ------------------------------------

void setup() {
  Serial.begin(9600);
  
  print_wakeup_reason();

  initIMU();

  Serial << "Ready" << endl;
}


void loop() {
  
  updateIMU();

  commandLine();

}


void commandLine() {
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
        IMU_PRINT_STATS = !IMU_PRINT_STATS;
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

        if(IMU_PRINT_STATS) Serial << "*";
        Serial << "p: IMU_PRINT_STATS" << endl;
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

void imuStateChangeCallback(uint8_t s) {
  Serial << millis() << " imu state changed" << endl;
  // do actions here
  switch(s) {
    case IMU_SETTLE:
    break;
    case IMU_CALIBRATE_HOME:
    break;
    case IMU_ACTIVE:
    break;
    case IMU_INACTIVE:
    break;
  }
}

void imuOrientationChangeCallback(uint8_t o) {
  Serial << millis() << " imu orientation changed" << endl;
  // do actions here
  switch(o) {
    case IMU_TABLETOP:
    break;
    case IMU_HANG:
    break;
    case IMU_UNKNOWN:
    break;
  }
}

void imuPoseChangeCallback(uint8_t p) {
  Serial << millis() << " imu pose changed" << endl;
  // do actions here
  switch(p) {
    case IMU_Pose_Tilt_L:
    break;
    case IMU_Pose_Tilt_R:
    break;
    case IMU_Pose_Tilt_Fwd:
    break;
    case IMU_Pose_Tilt_Bwd:
    break;
    case IMU_Pose_Home:
    break;
    case IMU_Pose_NA:
    break;
  }
}

void imuEventDetectedCallback(uint8_t e) {
  Serial << millis() << " imu event detected" << endl;
  // do actions here
  if(e) {
    // event detected
  } else {
    // no event
  }
}

