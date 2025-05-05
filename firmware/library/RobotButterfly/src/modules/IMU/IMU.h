#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg
#include "MPU6050.h"    // https://github.com/ElectronicCats/mpu6050/ 


// ------------ callbacks ------------
typedef void (*IMUCallback)(uint8_t); // imu callback type

extern IMUCallback onStateChangeCallback;
extern IMUCallback onOrientationChangeCallback;
extern IMUCallback onPoseChangeCallback;
extern IMUCallback onEventDetectedCallback;
// ------------------------------------

// --------------- api ---------------
// returns IMU_STATE which is an enum: IMUStates
uint8_t getIMUState();

// returns IMU_ORIENTATION which is an enum: IMUOrientations
uint8_t getIMUOrientation();

// returns IMU_POSE which is an enum: IMUPoses
uint8_t getIMUPose();
// -----------------------------------

// --------------- imu ----------------
static hw_timer_t *timer_10Hz_imu_cfg;
static volatile bool new_avg_sample;

static MPU6050 mpu;

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

enum IMUStates {
  IMU_SETTLE,
  IMU_CALIBRATE_HOME,
  IMU_ACTIVE,
  IMU_INACTIVE
};

static uint8_t IMU_STATE;
static uint8_t IMU_STATE_PREV;

enum IMUOrientations {
  IMU_TABLETOP,
  IMU_HANG,
  IMU_UNKNOWN
};

static uint8_t IMU_ORIENTATION;
static uint8_t IMU_ORIENTATION_PREV;
static long last_orientation_change;

enum IMUPoses {
  IMU_Pose_Tilt_L,
  IMU_Pose_Tilt_R,
  IMU_Pose_Tilt_Fwd,
  IMU_Pose_Tilt_Bwd,
  IMU_Pose_Home,
  IMU_Pose_NA
};

static uint8_t IMU_POSE;
static uint8_t IMU_POSE_PREV;
static long last_pose_detected;

static bool EVENT_DETECTED;
static long last_event_detected;
static long last_event_score_clear;
static int event_score;

static movingAvg imu_avg_data_ax(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_data_ay(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_data_az(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_data_gx(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_data_gy(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_data_gz(IMU_MOVING_AVG_WINDOW);

static movingAvg imu_avg_home_ax(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_home_ay(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_home_az(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_home_gx(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_home_gy(IMU_MOVING_AVG_WINDOW);
static movingAvg imu_avg_home_gz(IMU_MOVING_AVG_WINDOW);

static long last_imu_calib_print;
static long last_home_calibration;
static long settle_start;
static long calibration_start;
static long last_score_clear;
static int home_recalibrate_score;
static long last_imu_stats_print;

static bool IMU_PRINT_RAW;
static bool IMU_PRINT_DATA_AVG;
static bool IMU_PRINT_DELTA_HOME_AVG; // usually true during dev
static bool IMU_PRINT_DELTA_TIME_AVG;
static bool IMU_PRINT_STATS; // usually true during testing
// ------------------------------------

// ------------ functions -------------
void initIMU();
void updateIMU();
void Task_IMU_code(void * pvParameters);
void setIMUTaskPriority(uint8_t p);
// ------------------------------------

// ---------- rtos ------------
static TaskHandle_t Task_IMU;
static SemaphoreHandle_t Mutex_IMU;
static long last_imu_rtos_print;
// -------------------------------

#endif