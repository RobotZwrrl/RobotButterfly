/* Robot Butterfly RITP Performer A
 * ----------------------------------
 * The movements for the entire team are 
 * controlled by the conductor robot.
 * Sent through MQTT!
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);

#define ULTRASONIC_ACTIVE true
#define MAX_SEGMENTS  7

String topic_segments[MAX_SEGMENTS];
long last_sound_play = 0;
long last_movement_rest = 0;
uint8_t team = 0;
bool home_pos_active = false;
long last_pose_movement = 0;
bool initial_connection = false;
long last_ultrasonic_action = 0;


enum TeamNum {
  RED_TEAM,
  BLUE_TEAM,
  OTHER_TEAM
};


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
    team = RED_TEAM;
  } else if(robotbutterfly.getPreference(SETTINGS_TEAM) == "blue") {
    Serial << "blue team" << endl;
    team = BLUE_TEAM;
  } else {
    Serial << "other team" << endl;
    team = OTHER_TEAM;
  }

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

}


void loop() {
  robotbutterfly.update();
  console();

  if(ULTRASONIC_ACTIVE) ultrasonicAction();

  if(millis()-last_movement_rest >= 20000 && 
     millis() > 10000 &&
     millis()-last_pose_movement >= 500) {
    
      // restAction is synced with the conductor robot
      //restAction();

  }

}


void ultrasonicAction() {

  int proximity8 = getProximity8(&ultrasonic, true)+1; // boolean parameter = if sensor data is raw or averaged val
  
  if(proximity8 <= 5) {
    
    if(millis()-last_ultrasonic_action >= 1000) {

      Serial << "ultrasonic action" << endl;

      playSound(SOUND_ALERT_SNEEZE);
      robotbutterfly.conductTeam("6");
      neoAnimation2();

      setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
      setServoAnimSpeed(&servo_animation_alert, 100);
      setServoAnimRepeats(&servo_animation_alert, 2);
      startServoAnim(&servo_animation_alert);

      last_ultrasonic_action = millis();

    }
  }

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
          performIndividualAction(0);
        break;
        case '1':
          performIndividualAction(1);
        break;
        case '2':
          performIndividualAction(2);
        break;

        case 'q':
          performTeamAction(0);
        break;
        case 'w':
          performTeamAction(1);
        break;
        case 'e':
          performTeamAction(2);
        break;
        case 'r':
          performTeamAction(3);
        break;
        case 't':
          performTeamAction(4);
        break;
        case 'y':
          performTeamAction(5);
        break;
        case 'u':
          performTeamAction(6);
        break;

        case 'i':
          if(home_pos_active) {
            Serial << "\r\nhome_pos_active = true" << endl;
          } else {
            Serial << "\r\nhome_pos_active = false" << endl;
          }
        break;
        
        case 'h':
          Serial << "0-2: action - individual" << endl;
          Serial << "q-t: action - team" << endl;
          Serial << "i: home_pos_active" << endl;
          Serial << "c: iot connect" << endl;
          Serial << "s: iot disconnect" << endl;
          Serial << "h: help" << endl;
        break;
      }
    }
  }

}


