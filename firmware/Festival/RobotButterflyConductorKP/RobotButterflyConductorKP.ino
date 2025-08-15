/* Robot Butterfly Conductor
 * --------------------------
 * Respond to the IMU poses with different
 * light patterns and servo animations!
 * When at rest, flap the wings every 20s.
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);

void imuPoseChangeCallback(uint8_t p);

long last_movement_rest = 0;
uint8_t team = 0;
bool initial_connection = false;


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  
  robotbutterfly.init(true, false);

  robotbutterfly.onIOTMessageReceivedCallback_client = myiotMessageReceivedCallback;
  robotbutterfly.onIOTMessagePublishedCallback_client = myiotMessagePublishedCallback;
  robotbutterfly.onIOTConnectedCallback_client = myiotConnectedCallback;
  robotbutterfly.onIOTDisconnectedCallback_client = myiotDisconnectedCallback;

  robotbutterfly.onPoseChangeCallback_client = imuPoseChangeCallback;

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  if(robotbutterfly.getPreference(SETTINGS_TEAM) == "red") {
    Serial << "red team" << endl;
    team = 1;
  } else if(robotbutterfly.getPreference(SETTINGS_TEAM) == "blue") {
    Serial << "blue team" << endl;
    team = 2;
  }

  setTaskPriorities();

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

}

void loop() {
  
  robotbutterfly.update();
  console();

  if(getIMUState() != IMU_ACTIVE) {
    digitalWrite(LED_HEARTBEAT_PIN, LOW);
    digitalWrite(LED_COMMS_PIN, LOW);
  } else {
    digitalWrite(LED_HEARTBEAT_PIN, HIGH);
    digitalWrite(LED_COMMS_PIN, HIGH);
  }

  if(getIMUPose() == IMU_Pose_Home && getIMUState() == IMU_ACTIVE) {
    
    if(millis()-last_movement_rest >= 20000 && millis() > 10000) {
      
      restAction();
      last_movement_rest = millis();

    }
  }

}


void setTaskPriorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_LOW);
  setIMUTaskPriority(PRIORITY_IMU_MID);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_LOW);
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
          tiltLeftAction();
        break;
        case '2':
          //robotbutterfly.conductTeam("2"); // tilt right
          tiltRightAction();
        break;
        case '3':
          //robotbutterfly.conductTeam("3"); // tilt bwd
          tiltBwdAction();
        break;
        case '4':
          //robotbutterfly.conductTeam("4"); // tilt fwd
          tiltFwdAction();
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

