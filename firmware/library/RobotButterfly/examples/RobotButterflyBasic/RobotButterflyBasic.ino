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

  playSound(SOUND_ALERT_STARTUP);
}

void loop() {

  robotbutterfly.update(UPDATE_STATEMACHINE_ON, 
                        UPDATE_BUTTONS_ON, 
                        UPDATE_SOUND_OFF, 
                        UPDATE_IMU_OFF, 
                        UPDATE_NEOANIM_ON,
                        UPDATE_SERVOANIM_ON,
                        UPDATE_SENSORS_OFF,
                        UPDATE_PROXIMITY_OFF);

}

