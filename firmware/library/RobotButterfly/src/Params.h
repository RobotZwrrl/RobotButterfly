// --------- buttons ----------

#define DEBUG_BUTTONS                  false   // debug print statements
#define DEBOUNCE_TIME                    250   // (isr) time for the signal to settle
#define ACCIDENTAL_CLICK_TIME            200   // (isr) prevent bouncing double clicks
#define BUTTON_HOLD_DURATION            1000   // hold for this long for a long press in ms
#define BUTTON_HOLD_NOTIF_DURATION      1200   // timeout long press after this much ms
#define MAX_CLICK_TIME                   700   // amount of time between press and release to qualify as a click (in ms)
#define BUTTON_BOTH_RELEASE_LOCKOUT     1000   // amount of time to wait after both hold has been released to register another button hold
#define NEXT_INDIVIDUAL_BUTTON_HOLD      200   // amount of time after a both button hold til an individual button hold is registered

// ----------------------------

// ----------- imu ------------

// debug print statements
#define DEBUG_IMU false

// debug print statements on callbacks or other news
#define DEBUG_IMU_NEWS false

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

// ----------------------------

// ------ neo animations ------

#define DEBUG_NEO_COLOURS               false   // debug print statements
#define DEBUG_NEO_ANIMATION             false   // debug print statements
#define PREFS_NEO_BRIGHTNESS              20    // brightness 0-255. default: 20. this will be a preference
#define NUM_PALETTE_COLOURS               15    // number of palette colours
#define DEBUG_NEOANIM_NEWS              false    // printouts from the callbacks

// ----------------------------

// -------- proximity ---------

#define DEBUG_PROXIMITY     false
#define PROXIMITY_MIN          5   // cm
#define PROXIMITY_MAX         25   // cm
#define PROXIMITY_THRESHOLD   10   // cm
#define PROXIMITY_MOVING_AVG_WINDOW     5
#define PROXIMITY_TRIGGER_FREQ        500 // ms
#define PROXIMITY_WARMUP_TIME        2000 // ms

// ----------------------------

// ---------- sensor ----------

#define DEBUG_SENSORS   true
#define NUM_SENSORS        5

#define SENSOR_MOVING_AVG_VAL_WINDOW       10  // 1 sample every 0.1 seconds = 10 samples per 1 second
#define SENSOR_MOVING_AVG_AMBIENT_WINDOW   60  // 1 sample/s for 60 s = 60 samples

#define LIGHT_CHANGE_THRESH          300
#define LIGHT_AMBIENT_THRESH         150
#define LIGHT_WARMUP                5000   // 5 seconds from power on to let the sensor warm up before any triggers

#define SOUND_CHANGE_THRESH          200
#define SOUND_AMBIENT_THRESH         100
#define SOUND_WARMUP                5000   // 5 seconds from power on to let the sensor warm up before any triggers

#define TEMPERATURE_SENSOR_ENABLED  true   // convienient way to disable this sensor if needed
#define TEMPERATURE_CHANGE_THRESH      1   // .1 deg C
#define TEMPERATURE_AMBIENT_THRESH    20   // 2 deg C
#define TEMPERATURE_WARMUP         21000   // 20 seconds (avg duration) + 1 second from power on to let the sensor warm up before any triggers

#define HUMIDITY_SENSOR_ENABLED     true   // convienient way to disable this sensor if needed
#define HUMIDITY_CHANGE_THRESH        10   // 1% RH
#define HUMIDITY_AMBIENT_THRESH       20   // 2% RH
#define HUMIDITY_WARMUP            21000   // 20 seconds (avg duration) + 1 second from power on to let the sensor warm up before any triggers

// ----------------------------

// ------- servo anim ---------

#define DEBUG_SERVO               false   // debug print statements
#define DEBUG_SERVO_ANIM          false   // debug print statements
#define DEBUG_SERVOANIM_NEWS      false    // debug prints in callbacks

// ----------------------------

// ---------- sound -----------

#define DEBUG_SOUND              false   // debug print statements

// ----------------------------

// ------ state machine -------

#define DEBUG_STATEMACHINE       true
#define MAX_STATE                4
#define NUM_STATES 8

//#define TRANSITION_FRAME_TIME    100000    // 10 seconds
//#define TRANSITION_FRAME_TIME    50000     // 5 seconds
//#define TRANSITION_FRAME_TIME    40000     // 4 seconds
//#define TRANSITION_FRAME_TIME    30000     // 3 seconds
//#define TRANSITION_FRAME_TIME    20000     // 2 seconds
//#define TRANSITION_FRAME_TIME    10000     // 1 seconds
#define TRANSITION_FRAME_TIME    5000     // 0.5 seconds
//#define TRANSITION_FRAME_TIME    0         // 0 seconds

#define STATE_LOOP_PRINT         2000   // how long to print this every iteration of the state loop (in ms)

// ----------------------------
