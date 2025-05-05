#define RTOS_ENABLED       true

// ------ buttons rtos -------
#define STACK_BUTTONS            1500//1000   // task stack size
#define TASK_CORE_BUTTONS           0   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_BUTTONS           1   // delay until (ms), each loop
#define PRIORITY_BUTTONS_LOW        5
#define PRIORITY_BUTTONS_MID        8
#define PRIORITY_BUTTONS_HIGH       9
#define DEBUG_BUTTONS_RTOS      false

// ------ imu rtos -------
#define STACK_IMU            1800   // task stack size
#define TASK_CORE_IMU           1   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_IMU           5   // delay until (ms), each loop
#define PRIORITY_IMU_LOW        2
#define PRIORITY_IMU_MID        5
#define PRIORITY_IMU_HIGH       8
#define DEBUG_IMU_RTOS      false

// ------ neoanim rtos -------
#define STACK_NEOANIM            1800   // task stack size
#define TASK_CORE_NEOANIM           1   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_NEOANIM           1   // delay until (ms), each loop
#define PRIORITY_NEOANIM_LOW        7
#define PRIORITY_NEOANIM_MID        8
#define PRIORITY_NEOANIM_HIGH       9
#define DEBUG_NEOANIM_RTOS      false

// ------ proximity rtos -------
#define STACK_PROXIMITY            1250   // task stack size
#define TASK_CORE_PROXIMITY           0   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_PROXIMITY           5   // delay until (ms), each loop
#define PRIORITY_PROXIMITY_LOW        3
#define PRIORITY_PROXIMITY_MID        5
#define PRIORITY_PROXIMITY_HIGH       8
#define DEBUG_PROXIMITY_RTOS      false

// ------ sensors rtos -------
#define STACK_SENSORS            2000   // task stack size
#define TASK_CORE_SENSORS           1   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_SENSORS           5   // delay until (ms), each loop
#define PRIORITY_SENSORS_LOW        3
#define PRIORITY_SENSORS_MID        6
#define PRIORITY_SENSORS_HIGH       7
#define DEBUG_SENSORS_RTOS      false

// ------ servoanim rtos -------
#define STACK_SERVOANIM            2000   // task stack size
#define TASK_CORE_SERVOANIM           1   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_SERVOANIM           1   // delay until (ms), each loop
#define PRIORITY_SERVOANIM_LOW        6
#define PRIORITY_SERVOANIM_MID        8
#define PRIORITY_SERVOANIM_HIGH      10
#define DEBUG_SERVOANIM_RTOS      false

// ------ sound rtos -------
#define STACK_SOUND            1000   // task stack size
#define TASK_CORE_SOUND           0   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_SOUND           1   // delay until (ms), each loop
#define PRIORITY_SOUND_LOW        2
#define PRIORITY_SOUND_MID        4
#define PRIORITY_SOUND_HIGH       7
#define DEBUG_SOUND_RTOS      false



