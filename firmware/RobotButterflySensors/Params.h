// debug print statements
#define DEBUG_SENSORS true

#define NUM_SENSORS 5

// ambient values stored every 5 seconds, making the history
// of ambient readings 5*24 = 120 seconds = 2 minutes
#define NUM_AMBIENT_VALS 24

// number of samples to collect within a window of the 
// moving average filter
#define SENSOR_MOVING_AVG_RAW_WINDOW 20//10 // 1 sample every 0.1 seconds = 10 samples per 1 second 

#define SENSOR_MOVING_AVG_AMBIENT_WINDOW 10//5 // 1 sample every 1 second, = 5 samples for 5 seconds

#define LIGHT_CHANGE_THRESH 200



