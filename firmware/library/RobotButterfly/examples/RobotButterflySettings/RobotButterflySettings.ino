/* Robot Butterfly Settings
 * --------------------------
 * Read and write the settings, quickly!
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

String wifi_ssid = "";
String wifi_pass = "";
String mqtt_server = "";
String mqtt_port = "";
String mqtt_user = "";
String mqtt_pass = "";
String mqtt_id = "";
String robot_name = "";
String robot_namespace = "";
String robot_set = "";
String robot_team = "";
String iot_autoconnect = "";

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();

  robotbutterfly.init(true, true);

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);
}


void loop() {

  robotbutterfly.update();

  console();

}


void console() {

  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');
    if( robotbutterfly.processConsole(str) ) {
      char c = str[0];
      switch(c) {
        case 'r': 
          Serial << "-----------------" << endl;
          Serial << "read preferences" << endl;
          Serial << "-----------------" << endl;
          readPreferences();
        break;
        case 'w': {
          writePreferences();
          Serial << "-----------------" << endl;
          Serial << "wrote preferences" << endl;
          Serial << "-----------------" << endl;
          readPreferences();
        }
        break;
      }
    }
  }
  
}


void readPreferences() {
  preferences.begin("app", true);
  Serial << "wifi ssid: " << preferences.getString(SETTINGS_WIFI_SSID) << endl;
  Serial << "wifi pass: " << preferences.getString(SETTINGS_WIFI_PASS) << endl;
  Serial << "mqtt server: " << preferences.getString(SETTINGS_MQTT_SERVER) << endl;
  Serial << "mqtt port: " << preferences.getString(SETTINGS_MQTT_PORT) << endl;
  Serial << "mqtt user: " << preferences.getString(SETTINGS_MQTT_USER) << endl;
  Serial << "mqtt pass: " << preferences.getString(SETTINGS_MQTT_PASS) << endl;
  Serial << "mqtt id: " << preferences.getString(SETTINGS_MQTT_ID) << endl;
  Serial << "robot name: " << preferences.getString(SETTINGS_ROBOT_NAME) << endl;
  Serial << "robot namespace: " << preferences.getString(SETTINGS_NAMESPACE) << endl;
  Serial << "robot set: " << preferences.getString(SETTINGS_SET) << endl;
  Serial << "robot team: " << preferences.getString(SETTINGS_TEAM) << endl;
  Serial << "iot autoconnect: " << preferences.getString(SETTINGS_IOT_AUTOCONNECT) << endl;
  preferences.end();
}


void writePreferences() {
  preferences.begin("app", false);
  preferences.putString(SETTINGS_WIFI_SSID, wifi_ssid);
  preferences.putString(SETTINGS_WIFI_PASS, wifi_pass);
  preferences.putString(SETTINGS_MQTT_SERVER, mqtt_server);
  preferences.putString(SETTINGS_MQTT_PORT, mqtt_port);
  preferences.putString(SETTINGS_MQTT_USER, mqtt_user);
  preferences.putString(SETTINGS_MQTT_PASS, mqtt_pass);
  preferences.putString(SETTINGS_MQTT_ID, mqtt_id);
  preferences.putString(SETTINGS_ROBOT_NAME, robot_name);
  preferences.putString(SETTINGS_NAMESPACE, robot_namespace);
  preferences.putString(SETTINGS_SET, robot_set);
  preferences.putString(SETTINGS_TEAM, robot_team);
  preferences.putString(SETTINGS_IOT_AUTOCONNECT, iot_autoconnect);
  preferences.end();
}

