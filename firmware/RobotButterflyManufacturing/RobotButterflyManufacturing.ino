/*
 * Robot Butterfly Manufacturing
 * ------------------------------
 * Load the board with the right settings
 * before uploading the latest firmware
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
#include <Streaming.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include "Settings.h"
#include "WiFi.h"


// ------------ vars -------------
Preferences preferences;
long last_print;
long connect_time = 0;
// -------------------------------


void setup() {

  Serial.begin(9600);
  pinMode(LED_COMMS_PIN, OUTPUT);

  while(Serial.available() < 0) { ; } // wait for terminal

  delay(5000);

  print_wakeup_reason();

  Serial << "Ready" << endl;

  //eraseMem();
  
  delay(1000);
  Serial << "------- DONE -------" << endl;

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
      case 'w':
        writeDefaultPrefs();
      break;
      case 't':
        writeTestPrefs();
      break;
      case 'e':
        eraseMem();
      break;
      case 'c':
        connectWifi();
      break;
      case 'v':
        disconnectWifi();
      break;
      case 'h':
        Serial << "r: read prefs" << endl;
        Serial << "w: write default prefs" << endl;
        Serial << "t: write test prefs" << endl;
        Serial << "e: erase mem" << endl;
        Serial << "c: connect wifi" << endl;
        Serial << "v: disconnect wifi" << endl;
      break;
    }
  }

}


void readPrefs() {

  preferences.begin("app", true); // true means read only

  Serial << "wifi_ssid: " << preferences.getString("wifi_ssid") << endl;
  Serial << "wifi_password: " << preferences.getString("wifi_password") << endl;

  preferences.end();

}


void writeDefaultPrefs() {

  preferences.begin("app", false); // false means read-write
  preferences.putString("wifi_ssid", ssid_default);
  preferences.putString("wifi_password", password_default);
  preferences.end();
  Serial << "default preferences saved" << endl;

}


void writeTestPrefs() {

  preferences.begin("app", false); // false means read-write
  preferences.putString("wifi_ssid", ssid_internal);
  preferences.putString("wifi_password", password_internal);
  preferences.end();
  Serial << "test preferences saved" << endl;

}


void connectWifi() {

  preferences.begin("app", true); // true means read only
  String ssid = preferences.getString("wifi_ssid"); 
  String password = preferences.getString("wifi_password");
  preferences.end();

  if (ssid == "" || password == ""){
    Serial << "no values saved for wifi ssid or password" << endl;
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("connecting to wifi");
  connect_time = millis();

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    if(millis()-connect_time >= 10000) {
      Serial << "connection timed out" << endl;
      Serial << "did not connect to wifi" << endl;
      digitalWrite(LED_COMMS_PIN, LOW);
      //break;
      return;
    }
  }

  digitalWrite(LED_COMMS_PIN, HIGH);
  Serial << endl;
  Serial << "connected to wifi " << WiFi.localIP() << endl;

}


void disconnectWifi() {
  WiFi.disconnect();
  digitalWrite(LED_COMMS_PIN, LOW);
  delay(1000);
  Serial << "disconnected from wifi " << WiFi.localIP() << endl;
}


void eraseMem() {
  Serial << "erasing NVS partition... ";
  nvs_flash_erase();  // erase the NVS partition and...
  Serial << "done" << endl;
  Serial << "init NVS partition... ";
  nvs_flash_init();   // initialize the NVS partition.
  Serial << "done" << endl;
}

