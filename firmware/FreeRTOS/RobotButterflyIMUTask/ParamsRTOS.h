// ------ buttons rtos -------
#define STACK_IMU            1800   // task stack size
#define TASK_CORE_IMU           1   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_IMU           0   // delay until (ms), each loop
#define PRIORITY_IMU_LOW        2
#define PRIORITY_IMU_MID        6
#define PRIORITY_IMU_HIGH      10