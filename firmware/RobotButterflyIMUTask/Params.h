// --------- RTOS ----------
#define STACK_IMU            2048   // Task_IMU stack size
#define TASK_CORE_IMU           0
#define TASK_FREQ_IMU           0   // delay until (ms), each loop
#define PRIORITY_IMU_LOW        8
#define PRIORITY_IMU_MID        9
#define PRIORITY_IMU_HIGH      10

// debug print statements
#define DEBUG_IMU false

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
#define IMU_POSE_LOCKOUT 750

// dynamically allow for recalibration of the
// home position or not. it will always calibrate
// on startup. this will eventually be a user-
// definable preference.
#define PREFS_IMU_AUTO_RECALIBRATE_HOME false

// how long to 'hang on' to the previous orientation
// when the current orientation changes to unknown
#define IMU_ORIENTATION_CHANGE_LOCKOUT 3000

// how long to wait after the last event to check
// if there is a new event
#define IMU_EVENT_LOCKOUT_TIME 3000

// imu delta home value has to be greater than this
// value to increment the score for these two orientations
#define IMU_DELTA_EVENT_THRESH_HANG 300
#define IMU_DELTA_EVENT_THRESH_TABLETOP 100

// event score has to be greater than this number
// to be classified as an actual event
#define IMU_EVENT_SCORE_THRESH 4

// clear the event score this often
#define IMU_EVENT_SCORE_CLEAR 750

