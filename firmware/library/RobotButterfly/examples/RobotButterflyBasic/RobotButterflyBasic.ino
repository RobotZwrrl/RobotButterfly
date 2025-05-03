#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

// -- function prototypes --
void setupState1();
void loopState1();
// --

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  robotbutterfly.init();

  robotbutterfly.addState(RobotButterfly::STATE1, setupState1, loopState1);
  //robotbutterfly.changeState(RobotButterfly::STATE1);

  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);

  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // startServoAnim(&servo_animation_alert);
}

void loop() {
  robotbutterfly.update();
}
