/* Robot Butterfly Performer
 * --------------------------
 * Subscribes to the conductor commands
 * sent through MQTT and performs the
 * actions when on the same team!
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);

#define MAX_SEGMENTS  7
#define SONG_PLAY_DELAY 300

String topic_segments[MAX_SEGMENTS];
long last_sound_play = 0;
uint8_t team = 0;


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  
  robotbutterfly.init(true, false);

  robotbutterfly.onIOTMessageReceivedCallback_client = myiotMessageReceivedCallback;
  robotbutterfly.onIOTMessagePublishedCallback_client = myiotMessagePublishedCallback;
  robotbutterfly.onIOTConnectedCallback_client = myiotConnectedCallback;
  robotbutterfly.onIOTDisconnectedCallback_client = myiotDisconnectedCallback;

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  if(robotbutterfly.getPreference(SETTINGS_TEAM) == "red") {
    Serial << "red team" << endl;
    team = 1;
  } else if(robotbutterfly.getPreference(SETTINGS_TEAM) == "blue") {
    Serial << "blue team" << endl;
    team = 2;
  }

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
        case 'c':
          robotbutterfly.enableMQTT();
        break;
        case 's':
          robotbutterfly.disableMQTT();
        break;
        case '0':
          performAction(0);
        break;
        case '1':
          performAction(1);
        break;
        case '2':
          performAction(2);
        break;
        case '3':
          performAction(3);
        break;
        case '4':
          performAction(4);
        break;
        
        case 't':
          robotbutterfly.sendMQTTMessage( (robotbutterfly.getPreference(SETTINGS_MQTT_ID)+"/message"), "hello" );
        break;
        case 'y':
          robotbutterfly.sendMQTTMessage( (robotbutterfly.getPreference(SETTINGS_MQTT_ID)+"/message"), "world" );
        break;
        case 'h':
          Serial << "0-4: action" << endl;
          Serial << "c: iot connect" << endl;
          Serial << "s: iot disconnect" << endl;
          Serial << "h: help" << endl;
        break;
      }
    }
  }

}


