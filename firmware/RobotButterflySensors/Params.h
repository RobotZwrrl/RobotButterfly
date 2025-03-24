#define DEBUG_SENSORS   true
#define NUM_SENSORS        5

#define SENSOR_MOVING_AVG_VAL_WINDOW       10  // 1 sample every 0.1 seconds = 10 samples per 1 second 
#define SENSOR_MOVING_AVG_AMBIENT_WINDOW   60  // 1 sample/s for 60 s = 60 samples

#define LIGHT_CHANGE_THRESH        300
#define LIGHT_AMBIENT_THRESH       150
#define LIGHT_WARMUP              5000  // 5 seconds from power on to let the sensor warm up before any triggers

#define TEMPERATURE_CHANGE_THRESH    1
#define TEMPERATURE_AMBIENT_THRESH   3
#define TEMPERATURE_WARMUP       10000  // 10 seconds from power on to let the sensor warm up before any triggers


