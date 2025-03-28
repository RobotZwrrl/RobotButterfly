#define REFRESH_INTERVAL_MILLIS   20   // delay while waiting for servos to complete their movement

#define DEBUG_ANIM       true
#define ANIM_MAX         20      // max number of animation frames

#define DEBUG_SM         true    // Task_SM_code()
#define WARN_SM          true    // primarily for unhandled mutex exceptions
#define STARTUP_WAIT_SM  0       // wait for the servos to warm up
#define STACK_SM         16000   // Task_SM stack size

#define LUT_PARAMS       4

#define SERVO_MIN_DWELL  80      // min dwell time in ms