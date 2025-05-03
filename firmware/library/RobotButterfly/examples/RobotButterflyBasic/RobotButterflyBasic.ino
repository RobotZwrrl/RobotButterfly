#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  robotbutterfly.init();

  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);

  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // startServoAnim(&servo_animation_alert);
}

void loop() {
  robotbutterfly.update();
}
