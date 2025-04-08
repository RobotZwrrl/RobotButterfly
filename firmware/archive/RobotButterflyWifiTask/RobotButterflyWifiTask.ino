/*
 * Robot Butterfly Wifi Task
 * ------------------------------
 * Connect to wifi and reconnect when the
 * connection is lost. Implemented as a task
 * 
 * Note: To see LED_COMMS_PIN vs the power
 * LED on Keystone, the device has to be
 * tilted to see the 5mm LED through the 
 * window in the enclosure since both lights
 * are yellow in the inaugural version hehe.
 *
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2 or 2.0.4
 *
 * 20 Feb 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include <Preferences.h>
#include "WiFi.h"

// ------------ wifi -------------
static TaskHandle_t Task_WIFI;
static SemaphoreHandle_t Mutex_WIFI;

bool AUTO_RECONNECT_WIFI = true; // auto reconnect if disconnected
bool WIFI_CONNECTING = false;
bool JUST_CONNECTED = false;
long last_wifi_check = 0;
long last_print = 0;
long connect_time = 0;

struct WifiTaskMon {
  long task_enter;
  long task_exit;
  uint8_t task_priority;
};

struct WifiTaskMon wifiTaskMon;

Preferences preferences;
// -------------------------------


void setup() {

  Serial.begin(9600);
  
  print_wakeup_reason();

  initWifi();

  Serial << "Ready" << endl;

}


void loop() {
  
  // if(millis()-last_print >= 1000) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'r':
        readPrefs();
      break;
      case 'c':
        connectWifi();
      break;
      case 'd':
        WIFI_CONNECTING = false;
        disconnectWifi();
      break;
      case 'b':
        AUTO_RECONNECT_WIFI = !AUTO_RECONNECT_WIFI;
        if(AUTO_RECONNECT_WIFI) {
          Serial << "AUTO_RECONNECT_WIFI == true" << endl;
        } else {
          Serial << "AUTO_RECONNECT_WIFI == false" << endl;
        }
      break;
      case 'm':
        readMacAddress();
      break;
      case 'i':
        Serial << "ip address: " << WiFi.localIP() << endl;
      break;
      case '[':
        vTaskPrioritySet(Task_WIFI, PRIORITY_WIFI_LOW);
        Serial << "PRIORITY_WIFI_LOW" << endl;
      break;
      case ']':
        vTaskPrioritySet(Task_WIFI, PRIORITY_WIFI_MID);
        Serial << "PRIORITY_WIFI_MID" << endl;
      break;
      case '\\':
        vTaskPrioritySet(Task_WIFI, PRIORITY_WIFI_HIGH);
        Serial << "PRIORITY_WIFI_HIGH" << endl;
      break;
      case '=':
        vTaskPrioritySet(Task_WIFI, tskIDLE_PRIORITY);
        Serial << "tskIDLE_PRIORITY" << endl;
      break;
      case 'p': {
        uint8_t p = wifiTaskMon.task_priority;
        long t1 = wifiTaskMon.task_enter;
        long t2 = wifiTaskMon.task_exit;
        Serial << "Task priority: " << p << endl;
        Serial << "Task enter: " << t1 << endl;
        Serial << "Task exit: " << t2 << endl;
        Serial << "Task time: " << t2-t1 << endl;
      }
      break;
      case 'h':
        Serial << "r: read prefs" << endl;
        Serial << "c: connect wifi" << endl;
        Serial << "d: disconnect wifi" << endl;
        Serial << "b: toggle AUTO_RECONNECT_WIFI" << endl;
        Serial << "m: read mac address" << endl;
        Serial << "i: read ip address" << endl;
        Serial << "--- rtos ---" << endl;
        Serial << "[: set priority low" << endl;
        Serial << "]: set priority mid" << endl;
        Serial << "\\: set priority high" << endl;
        Serial << "=: set priority idle" << endl;
        Serial << "p: task mon" << endl;
      break;
    }
  }

}

