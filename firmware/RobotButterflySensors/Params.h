#define DEBUG_SENSORS   true
#define NUM_SENSORS        5

#define SENSOR_MOVING_AVG_VAL_WINDOW       10  // 1 sample every 0.1 seconds = 10 samples per 1 second 
#define SENSOR_MOVING_AVG_AMBIENT_WINDOW   60  // 1 sample/s for 60 s = 60 samples

#define LIGHT_CHANGE_THRESH          300
#define LIGHT_AMBIENT_THRESH         150
#define LIGHT_WARMUP                5000  // 5 seconds from power on to let the sensor warm up before any triggers

#define SOUND_CHANGE_THRESH          200
#define SOUND_AMBIENT_THRESH         100
#define SOUND_WARMUP                5000  // 5 seconds from power on to let the sensor warm up before any triggers

#define TEMPERATURE_CHANGE_THRESH      1   // .1 deg C
#define TEMPERATURE_AMBIENT_THRESH    20   // 2 deg C
#define TEMPERATURE_WARMUP         21000   // 20 seconds (avg duration) + 1 second from power on to let the sensor warm up before any triggers

#define HUMIDITY_CHANGE_THRESH        10   // 1% RH
#define HUMIDITY_AMBIENT_THRESH       20   // 2% RH
#define HUMIDITY_WARMUP            21000   // 20 seconds (avg duration) + 1 second from power on to let the sensor warm up before any triggers

