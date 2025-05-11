/* Robot Butterfly Basic - RTOS Demo
 * ----------------------------------
 * Here's a demo of how modifying task
 * priority parameters (using FreeRTOS) 
 * changes how Robot Butterfly performs
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

// -- function prototypes --
void setupState1();
void loopState1();
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
bool hold_notif_action = false;
// -------------------------------

void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  robotbutterfly.init(true, false);

  robotbutterfly.addState(RobotButterfly::STATE1, setupState1, loopState1);
  robotbutterfly.changeState(RobotButterfly::STATE1);

  robotbutterfly.onHoldNotificationCallback_client = buttonHoldNotificationCallback;
  robotbutterfly.onHoldReleasedCallback_client = buttonHoldReleasedCallback;
  robotbutterfly.onClickCallback_client = buttonClickCallback;

  demoTaskSet1();

  playSound(SOUND_ALERT_STARTUP);
}

void loop() {

  if(RTOS_ENABLED) {
    robotbutterfly.update();
  } else {
    robotbutterfly.update(UPDATE_STATEMACHINE_ON, 
                        UPDATE_BUTTONS_ON, 
                        UPDATE_SOUND_ON, 
                        UPDATE_IMU_OFF, 
                        UPDATE_NEOANIM_ON,
                        UPDATE_SERVOANIM_ON,
                        UPDATE_SENSORS_OFF,
                        UPDATE_PROXIMITY_OFF);
  }

  console();

}

void console() {

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '1':
        demoTaskSet1();
      break;
      case '2': 
        demoTaskSet2();
      break;
      case '3':
        demoTaskSet3();
      break;
      case '4':
        demoTaskSet4();
      break;
    }
  }

}

void demoTaskSet1() {
  Serial << "demo task set 1" << endl;
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_HIGH);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

void demoTaskSet2() {
  Serial << "demo task set 2" << endl;
  setButtonsTaskPriority(PRIORITY_BUTTONS_HIGH);
  setIMUTaskPriority(PRIORITY_IMU_HIGH);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_HIGH);
  setProximityTaskPriority(PRIORITY_PROXIMITY_HIGH);
  setSensorsTaskPriority(PRIORITY_SENSORS_HIGH);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_HIGH);
}

void demoTaskSet3() {
  Serial << "demo task set 3" << endl;
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(PRIORITY_IMU_LOW);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_HIGH);
  setProximityTaskPriority(PRIORITY_PROXIMITY_LOW);
  setSensorsTaskPriority(PRIORITY_SENSORS_LOW);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

void demoTaskSet4() {
  Serial << "demo task set 4" << endl;
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(PRIORITY_IMU_LOW);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_HIGH);
  setProximityTaskPriority(PRIORITY_PROXIMITY_LOW);
  setSensorsTaskPriority(PRIORITY_SENSORS_LOW);
  setServoAnimationTaskPriority(tskIDLE_PRIORITY);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

