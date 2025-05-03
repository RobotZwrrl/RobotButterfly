#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  robotbutterfly.init();

  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);
}

void loop() {
  robotbutterfly.update();
}
