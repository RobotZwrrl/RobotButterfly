#define DEBUG_IMU                       true   // debug print statements
#define MAX_IMU_ACCE 16384
#define MIN_IMU_ACCE -16384
#define MAX_IMU_GYRO 16384
#define MIN_IMU_GYRO -16384

#define IMU_MOVING_AVG_WINDOW 100 // 100 samples in 0.1 seconds = 1 sample / ms

#define DELTA_HOME true

#define IMU_HOME_SETTLE_TIME 3000
#define IMU_HOME_CALIBRATION_TIME 3000
