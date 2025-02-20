/*
 * Robot Butterfly Wifi
 * ------------------------------
 * Connect to wifi and reconnect when the
 * connection is lost
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
#include "WiFi.h"


// ------------ vars -------------
#define DEBUG_WIFI                true
#define WIFI_CHECK_INTERVAL       3000
#define WIFI_CONNECTION_TIMEOUT   10000
bool AUTO_RECONNECT_WIFI = true; // auto reconnect if disconnected
bool WIFI_CONNECTING = false;
long last_wifi_check = 0;
long last_print = 0;
long connect_time = 0;

Preferences preferences;
// -------------------------------


void setup() {

  Serial.begin(9600);
  pinMode(LED_COMMS_PIN, OUTPUT);

  print_wakeup_reason();

  Serial << "Ready" << endl;

  connectWifi();

}


void loop() {
  
  // if(millis()-last_print >= 1000) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  if(AUTO_RECONNECT_WIFI) {
    updateWifi();
  }

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
      case 'h':
        Serial << "r: read prefs" << endl;
        Serial << "c: connect wifi" << endl;
        Serial << "d: disconnect wifi" << endl;
        Serial << "b: toggle AUTO_RECONNECT_WIFI" << endl;
        Serial << "m: read mac address" << endl;
        Serial << "i: read ip address" << endl;
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


void connectWifi() {

  preferences.begin("app", true); // true means read only
  String ssid = preferences.getString("wifi_ssid"); 
  String password = preferences.getString("wifi_password");
  preferences.end();

  if (ssid == "" || password == ""){
    if(DEBUG_WIFI) Serial << "no values saved for wifi ssid or password" << endl;
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial << "connecting to wifi " << endl;
  connect_time = millis();
  WIFI_CONNECTING = true;
}


void disconnectWifi() {
  WiFi.disconnect();
  digitalWrite(LED_COMMS_PIN, LOW);
  if(DEBUG_WIFI) Serial << "disconnected from wifi " << WiFi.localIP() << endl;
}


void updateWifi() {

  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_COMMS_PIN, HIGH);
    if(WIFI_CONNECTING) {
      WIFI_CONNECTING = false;
      if(DEBUG_WIFI) Serial << endl;
      if(DEBUG_WIFI) Serial << "connected to wifi " << WiFi.localIP() << endl;
    }
    return;
  }

  if(WIFI_CONNECTING) {

    if(millis()-last_print >= 1000) {
      Serial << ".";
      last_print = millis();
    }

    if(millis()-connect_time >= WIFI_CONNECTION_TIMEOUT) {
      if(DEBUG_WIFI) Serial << "connection timed out" << endl;
      if(DEBUG_WIFI) Serial << "did not connect to wifi" << endl;
      digitalWrite(LED_COMMS_PIN, LOW);
      WIFI_CONNECTING = false;
      return;
    }

  }

  // auto reconnect when not already connecting
  if(AUTO_RECONNECT_WIFI == true && WIFI_CONNECTING == false) {
    if(millis()-last_wifi_check >= WIFI_CHECK_INTERVAL) {
      if(DEBUG_WIFI) Serial << millis() << " reconnecting to wifi" << endl;
      disconnectWifi();
      connectWifi();
      last_wifi_check = millis();
    }
  }

}


