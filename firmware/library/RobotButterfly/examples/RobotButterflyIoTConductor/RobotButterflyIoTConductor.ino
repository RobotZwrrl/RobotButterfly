/* Robot Butterfly Dance
 * --------------------------
 * Respond to the IMU poses with different
 * light patterns and servo animations!
 * when at rest, flap the wings every 20s.
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);


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
        case '1':
          robotbutterfly.conductNamespace("1");
        break;
        case '2':
          robotbutterfly.conductSet("1");
        break;
        case '3':
          robotbutterfly.conductTeam("1");
        break;
        case 'q':
          robotbutterfly.conductNamespace("2");
        break;
        case 'w':
          robotbutterfly.conductSet("2");
        break;
        case 'e':
          robotbutterfly.conductTeam("2");
        break;
        
        case 'h':
          Serial << "1,2,3: conduct a" << endl;
          Serial << "q,w,e: conduct b" << endl;
          Serial << "c: iot connect" << endl;
          Serial << "s: iot disconnect" << endl;
          Serial << "h: help" << endl;
        break;
      }
    }
  }

}
