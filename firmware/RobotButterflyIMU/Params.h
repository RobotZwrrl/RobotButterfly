#define DEBUG_IMU                       true   // debug print statements
#define MAX_IMU_ACCE 16384
#define MIN_IMU_ACCE -16384
#define MAX_IMU_GYRO 16384
#define MIN_IMU_GYRO -16384

// number of samples to collect within a window
// of the moving average filter. the window is
// complete when the hardware timer fires - it
// may not be the exact number defined here
#define IMU_MOVING_AVG_WINDOW 100 // 100 samples in 0.1 seconds = 1 sample / ms

// wait time for the imu to settle before using
// the values to make the home average (eg, if 
// they just placed the robot down after pressing
// the power switch)
#define IMU_HOME_SETTLE_TIME 2000

// wait time for collecting samples for the imu
// home position
#define IMU_HOME_CALIBRATION_TIME 3000

// amount of time to keep the home position for
// before recalibrating (if necessary)
#define IMU_HOME_RECALIBRATION_LOCKOUT_TIME 5000

// if the home imu delta exceeds this amount,
// increment the score to possibly trigger a
// home recalibration
#define IMU_DELTA_RECALIBRATE_HOME_THRESH 20

// if home_recalibrate_score exceeds this amount,
// it triggers the home recalibration
#define IMU_HOME_SCORE_THRESH 10

// amount of time since the previous score
// clearing to clear the score to prevent
// instantaneous anomolies from triggering a
// home recalibration
#define IMU_HOME_RECAL_SCORE_CLEAR 3000

// amount of time to wait after the last pose
// detected before checking to recalibrate the
// home position
#define IMU_POSE_LOCKOUT 10000

// dynamically allow for recalibration of the
// home position or not. it will always calibrate
// on startup. this will eventually be a user-
// definable preference.
#define PREFS_AUTO_RECALIBRATE_HOME true


