/* Robot Butterfly Integrated
 * --------------------------
 * Here's an example that integrates various
 * peripherals on Robot Butterfly to make
 * 4 routines! RTOS mode enabled
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
void buttonHoldNotificationCallback(uint8_t n);
void buttonHoldReleasedCallback(uint8_t n);
void buttonClickCallback(uint8_t n);
// --

// ----- servo calibration -------
bool SERVO_CAL_MODE = false;
bool left_cal_dir = true;
int left_cal_mode = 0;
bool right_cal_dir = true;
int right_cal_mode = 0;
bool button_calib_changed = false;
int servo_calib_pos_left = 0;
int servo_calib_pos_right = 0;
// -------------------------------

// ----------- other -------------
bool BATTERY_AA_MODE = true;
long start_del = 0;
bool hold_notif_action = false;
// -------------------------------


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();

  // start up robot butterfly with some settings:
  // 1st param false = we will init servos,
  // 2nd param false = we will increment statemachine
  robotbutterfly.init(false, false);

  // this playing multiple times on startup is an
  // indicator that the robot is being powered by
  // AA batteries because a few brownouts will
  // occur as the robot stabilises on startup when
  // moving its wings to the initial position.
  // having this sound helps for debugging and also
  // gives a cheeky characteristic (sounds like a 
  // robot giggling haha).
  playSound(SOUND_FLUTTER_JOY);

  setNeoAnim(&neo_animation_home, NEO_ANIM_FUNKY, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_SKY_BLUE);
  setNeoAnimSpeed(&neo_animation_home, 300);
  startNeoAnim(&neo_animation_home);

  start_del = millis();
  while(millis()-start_del < 300) {
    updateNeoAnimation();
    updateSound();
    delay(1);
  }

  // initialise the servos depending on if the AA
  // batteries are in use or not. if they are,
  // take some time to initialise each side
  batteryCheck(); 

  // turn some of unused tasks to idle while
  // the robot is starting up
  setInitialPriorities();

  // there are 4 routines in this sketch
  robotbutterfly.addState(RobotButterfly::STATE1, setupState1, loopState1);  // state 1: low power periodic flutter
  robotbutterfly.addState(RobotButterfly::STATE2, setupState2, loopState2);  // state 2: colourful fluttering
  robotbutterfly.addState(RobotButterfly::STATE3, setupState3, loopState3);  // state 3: rainbow sparkle flutter
  robotbutterfly.addState(RobotButterfly::STATE4, setupState4, loopState4);  // state 4: golden proximity interaction
  
  robotbutterfly.changeState(RobotButterfly::STATE1);

  // here are the callbacks that are implemented
  robotbutterfly.onHoldNotificationCallback_client = buttonHoldNotificationCallback;
  robotbutterfly.onHoldReleasedCallback_client = buttonHoldReleasedCallback;
  robotbutterfly.onClickCallback_client = buttonClickCallback;

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);
}


void loop() {

  robotbutterfly.update();

  // servo calibration mode 'overwrites' all the states
  if(SERVO_CAL_MODE) {

    if(button_calib_changed) {

      if(left_cal_mode == 0) { // wing up
        servo_calib_pos_left = SERVO_ANIM_POSITION_UP;
      } else if(left_cal_mode == 1) { // wing home
        servo_calib_pos_left = SERVO_ANIM_POSITION_HOME;
      } else if(left_cal_mode == 2) { // wing down
        servo_calib_pos_left = SERVO_ANIM_POSITION_DOWN;
      }

      if(right_cal_mode == 0) { // wing up
        servo_calib_pos_right = SERVO_ANIM_POSITION_UP;
      } else if(right_cal_mode == 1) { // wing home
        servo_calib_pos_right = SERVO_ANIM_POSITION_HOME;
      } else if(right_cal_mode == 2) { // wing down
        servo_calib_pos_right = SERVO_ANIM_POSITION_DOWN;
      }

      setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
      setServoAnimRepeats(&servo_animation_alert, -99);
      setServoAnimPositionLeft(&servo_animation_alert, servo_calib_pos_left);
      setServoAnimPositionRight(&servo_animation_alert, servo_calib_pos_right);
      startServoAnim(&servo_animation_alert);

      setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
      setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_GREEN);
      setNeoAnimSpeed(&neo_animation_alert, 1000);
      setNeoAnimUno(&neo_animation_alert, left_cal_mode); // top-most leds near the back
      setNeoAnimDuo(&neo_animation_alert, 5+right_cal_mode); // bottom-most leds near the front
      startNeoAnim(&neo_animation_alert);

      button_calib_changed = false;
    }

  }

  console();

}


void setInitialPriorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}


void batteryCheck() {

  int battery_val = analogRead(BATT_SIG_PIN);
  if(battery_val > 0) {
    Serial << battery_val << " BATTERY_AA_MODE enabled" << endl;
    BATTERY_AA_MODE = true;
  } else {
    Serial << battery_val << " BATTERY_AA_MODE disabled" << endl;
    BATTERY_AA_MODE = false;
  }

  if(BATTERY_AA_MODE == true) {

    initServos(SERVO_MODE_INIT_LEFT);
    
    // battery catch up
    // update can be called here from setup() as the
    // rtos scheduler has not started yet - interesting!
    setNeoAnimColours(&neo_animation_home, NEO_ORANGE, NEO_SKY_BLUE);
    startNeoAnim(&neo_animation_home);
    start_del = millis();
    while(millis()-start_del < 1200) {
      updateNeoAnimation();
      delay(1);
    }

    initServos(SERVO_MODE_INIT_RIGHT);

    // battery catch up
    // update can be called here from setup() as the
    // rtos scheduler has not started yet - interesting!
    setNeoAnimColours(&neo_animation_home, NEO_CANARY_YELLOW, NEO_GREEN);
    startNeoAnim(&neo_animation_home);
    start_del = millis();
    while(millis()-start_del < 1200) {
      updateNeoAnimation();
      delay(1);
    }

    initialised_servos = true; // remember to set the flag when initialising motors separately
    initServoAnimations();

  } else {

    initServos(SERVO_MODE_INIT_BOTH);
    initServoAnimations();

  }

}


void console() {

  if(Serial.available()) {
    char c = Serial.read();
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

