/* Robot Butterfly Blink
 * --------------------------
 * Get started with blinking the two LEDs
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  
  robotbutterfly.init(true, false);

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

}

void loop() {
  
  robotbutterfly.update();

  digitalWrite(LED_HEARTBEAT_PIN, LOW);
  digitalWrite(LED_COMMS_PIN, LOW);
  delay(100);
  digitalWrite(LED_HEARTBEAT_PIN, HIGH);
  digitalWrite(LED_COMMS_PIN, HIGH);
  delay(100);

}

