/*
 * Robot Butterfly Manufacturing
 * ------------------------------
 * Load the board with the right settings
 * before uploading the latest firmware
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
#include <Streaming.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include "Settings.h"


// ------------ vars -------------
Preferences preferences;

long last_print;
// -------------------------------


void setup() {

  Serial.begin(9600);

  while(Serial.available() < 0) { ; } // wait for terminal

  delay(5000);

  print_wakeup_reason();

  Serial << "Ready" << endl;

  Serial << "erasing NVS partition... ";
  nvs_flash_erase();  // erase the NVS partition and...
  Serial << "done" << endl;
  Serial << "init NVS partition... ";
  nvs_flash_init();   // initialize the NVS partition.
  Serial << "done" << endl;


  delay(1000);


  preferences.begin("credentials", false); // false means read-write
  preferences.putString("ssid", ssid); 
  preferences.putString("password", password);
  preferences.end();
  Serial << "network credentials saved" << endl;


  delay(1000);


  Serial << "------- DONE -------" << endl;

}


void loop() {
  
  if(millis()-last_print >= 1000) {
    Serial << millis() << " hi " << xPortGetCoreID() << endl;
    last_print = millis();
  }

}

