// ------ sound rtos -------
#define STACK_SOUND            1000   // task stack size
#define TASK_CORE_SOUND           0   // 0 has wdt, 1 does not have wdt
#define TASK_FREQ_SOUND           1   // delay until (ms), each loop
#define PRIORITY_SOUND_LOW        2
#define PRIORITY_SOUND_MID        4
#define PRIORITY_SOUND_HIGH       7