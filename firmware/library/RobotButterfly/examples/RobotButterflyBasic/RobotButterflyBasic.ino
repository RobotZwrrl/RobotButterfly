#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  robotbutterfly.init();

}

void loop() {
  robotbutterfly.update();
}

