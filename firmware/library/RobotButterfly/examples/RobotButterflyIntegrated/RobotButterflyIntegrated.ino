/* Robot Butterfly Integrated
 * --------------------------
 * Here's an example that integrates various
 * peripherals on Robot Butterfly to make
 * 4 routines!
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

// -- function prototypes --
void setupState1();
void loopState1();
void setupState2();
void loopState2();
void setupState3();
void loopState3();
void setupState4();
void loopState4();
// --


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();

  // muteSound(true); // uncomment this if you do not want sound

  // start up robot butterfly with some settings:
  // 1st param true = library will initialise the servos automatically
  // 2nd param true = library will increment state machine on button click automatically
  robotbutterfly.init(true, true);

  // there are 4 routines in this sketch
  robotbutterfly.setNumStates(4);
  robotbutterfly.addState(RobotButterfly::STATE1, setupState1, loopState1);  // state 1: low power periodic flutter
  robotbutterfly.addState(RobotButterfly::STATE2, setupState2, loopState2);  // state 2: colourful fluttering
  robotbutterfly.addState(RobotButterfly::STATE3, setupState3, loopState3);  // state 3: rainbow sparkle flutter
  robotbutterfly.addState(RobotButterfly::STATE4, setupState4, loopState4);  // state 4: golden proximity interaction
  
  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

  // enter in to the first state
  robotbutterfly.changeState(RobotButterfly::STATE1);
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
        case '1':
          robotbutterfly.changeState(RobotButterfly::STATE1);
        break;
        case '2':
          robotbutterfly.changeState(RobotButterfly::STATE2);
        break;
        case '3':
          robotbutterfly.changeState(RobotButterfly::STATE3);
        break;
        case '4':
          robotbutterfly.changeState(RobotButterfly::STATE4);
        break;

        case 'a':
          robotbutterfly.decrementState();
        break;
        case 'd':
          robotbutterfly.incrementState();
        break;

        case 'h':
          Serial << "1-4: states" << endl;
          Serial << "a: r button" << endl;
          Serial << "d: l button" << endl;
          Serial << "h: help" << endl;
        break;
      }
    }
  }

}

