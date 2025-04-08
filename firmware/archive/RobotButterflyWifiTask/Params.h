#define DEBUG_WIFI                 true
#define WIFI_CHECK_INTERVAL        3000   // how often to try to reconnect (+ WIFI_CONNECTION_TIMEOUT)
#define WIFI_CONNECTION_TIMEOUT   10000   // how long to wait while connecting
bool WIFI_CONNECT_ON_STARTUP  =    true;  // auto connect on startup

// --------- RTOS ----------
#define STACK_WIFI            2048   // Task_WIFI stack size
#define TASK_CORE_WIFI           0
#define TASK_FREQ_WIFI           1   // delay until (ms), each loop
#define PRIORITY_WIFI_LOW        2
#define PRIORITY_WIFI_MID        5
#define PRIORITY_WIFI_HIGH      10
