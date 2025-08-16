/* Robot Butterfly Conductor B
 * ----------------------------
 * Respond to the proximity sensor with
 * moving pixels and sounds!
 * When at rest, flap the wings every 20s.
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);

long last_movement_rest = 0;
uint8_t team = 0;
bool initial_connection = false;
uint8_t level = 0;
uint8_t level_prev = 0;
long last_toggle = 0;
bool prox_animation = true;
bool rest_animation = false;


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
  } else {
    Serial << "unknown team" << endl;
    team = 0;
  }

  setTaskPriorities();

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

  neoAnimationProximity(0);
}

void loop() {
  
  robotbutterfly.update();
  console();

  int proximity8 = getProximity8(&ultrasonic, true)+1; // boolean parameter = if sensor data is raw or averaged val
  
  if(proximity8 < 8) {
    if(!prox_animation) {
      neoAnimationProximity(proximity8);
      prox_animation = true;
      rest_animation = false;
    }
    setNeoAnimRangeVal(&neo_animation_home, proximity8);
  } else {
    if(millis()-last_toggle >= 500 && level == 0) {
      prox_animation = false;
      if(!rest_animation) {
        restNeo();
        rest_animation = true;
      }
    }
  }

  level_prev = level; 

  if(proximity8 <= 2) {
    level = 3;
  } else if(proximity8 > 2 && proximity8 <= 5) {
    level = 2;
  } else if(proximity8 > 5 && proximity8 <= 7) {
    level = 1;
  } else {
    level = 0;
  }

  if(level_prev != level && level != 0) {
    Serial << "level: " << level << endl;

    if(team == 1) {
      // flutter right up, left home stationary
      setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
      setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_UP);
      setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_HOME);
      setServoAnimDuration(&servo_animation_alert, 2000);
      setServoAnimSpeed(&servo_animation_alert, 200);
      startServoAnim(&servo_animation_alert);
    } else if(team == 2) { // blue
      // flutter left up, right home stationary
      setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
      setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_LEFT_UP);
      setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_HOME);
      setServoAnimDuration(&servo_animation_alert, 2000);
      setServoAnimSpeed(&servo_animation_alert, 200);
      startServoAnim(&servo_animation_alert);
    }

    if(level == 3) {
      robotbutterfly.conductTeam("11");
      if(team == 1) { // red
        playSound(SOUND_ALERT_WIFI_CONNECT);
      } else if(team == 2) { // blue
        playSound(SOUND_ALERT_WIFI_DISCONNECT);
      }
    } else if(level == 2) {
      robotbutterfly.conductTeam("12");
      if(team == 1) { // red
        playSound(SOUND_ALERT_MQTT_SEND);
      } else if(team == 2) { // blue
        playSound(SOUND_ALERT_MQTT_RECEIVE);
      }
    } else if(level == 1) {
      robotbutterfly.conductTeam("13");
      if(team == 1) { // red
        playSound(SOUND_ALERT_SNEEZE);
      } else if(team == 2) { // blue
        playSound(SOUND_ALERT_SNEEZE);
      }
    }
    last_toggle = millis();
  }


  if(millis()-last_movement_rest >= 20000 && millis() > 10000 && millis()-last_toggle >= 500) {
    restAction();
    last_movement_rest = millis();
  }


}


void setTaskPriorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_LOW);
  setIMUTaskPriority(PRIORITY_IMU_LOW);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(PRIORITY_PROXIMITY_MID);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_MID);
  setSoundTaskPriority(PRIORITY_SOUND_HIGH);
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
          //robotbutterfly.conductTeam("0"); // home position
          restAction();
        break;
        case '1':
          //robotbutterfly.conductTeam("1"); // tilt left
          
        break;
        case '2':
          //robotbutterfly.conductTeam("2"); // tilt right
          
        break;
        case '3':
          //robotbutterfly.conductTeam("3"); // tilt bwd
          
        break;
        case '4':
          //robotbutterfly.conductTeam("4"); // tilt fwd
          
        break;
        
        case 'h':
          Serial << "0-4: conduct team" << endl;
          Serial << "c: iot connect" << endl;
          Serial << "s: iot disconnect" << endl;
          Serial << "h: help" << endl;
        break;
      }
    }
  }

}

