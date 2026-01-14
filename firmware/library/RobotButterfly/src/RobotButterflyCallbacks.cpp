#include "RobotButterfly.h"

// ----------------------------------
// --------- mqtt callbacks -------
// ----------------------------------

void RobotButterfly::iotMessageReceivedCallback(String topic, String payload) {
	if(onIOTMessageReceivedCallback_client != NULL) onIOTMessageReceivedCallback_client(topic, payload);	
}

void RobotButterfly::iotMessagePublishedCallback(String topic, String payload) {
	if(onIOTMessagePublishedCallback_client != NULL) onIOTMessagePublishedCallback_client(topic, payload);	
}

void RobotButterfly::iotConnectedCallback() {
	if(onIOTConnectedCallback_client != NULL) onIOTConnectedCallback_client();	
}

void RobotButterfly::iotDisconnectedCallback() {
	if(onIOTDisconnectedCallback_client != NULL) onIOTDisconnectedCallback_client();	
}

// ----------------------------------


// ----------------------------------
// --------- button callbacks -------
// ----------------------------------

ButtonCallback RobotButterfly::onHoldNotificationCallback_client = NULL;
ButtonCallback RobotButterfly::onHoldReleasedCallback_client = NULL;
ButtonCallback RobotButterfly::onClickCallback_client = NULL;
ButtonCallback RobotButterfly::onReleaseCallback_client = NULL;


// give user feedback that they have held the
// button and its time to to release the button
void RobotButterfly::buttonHoldNotificationCallback(uint8_t n) {
  
  switch(n) {
    case BUTTON_BOTH:
      playSimpleTone(NOTE_F5, 500);
      playNoTone();
    break;
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 500);
      playNoTone();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 500);
      playNoTone();
    break;
  }


  if(n == BUTTON_BOTH) {

    if(!hold_notif_action) {

      setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
      setServoAnim(&servo_animation_home, SERVO_ANIM_NONE, SERVO_ANIM_HOME);

      setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
      setServoAnimRepeats(&servo_animation_alert, -99);
      setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
      setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
      startServoAnim(&servo_animation_alert);

      setNeoAnim(&neo_animation_alert, NEO_ANIM_ZWOOP, NEO_ANIM_ALERT);
      
      if(SERVO_CAL_MODE) {
        setNeoAnimColours(&neo_animation_alert, NEO_PURPLE, NEO_OFF);
      } else {
        setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_OFF);
      }
      
      setNeoAnimSpeed(&neo_animation_alert, 500);
      startNeoAnim(&neo_animation_alert);
      hold_notif_action = true;
    }

  }


  if(onHoldNotificationCallback_client != NULL) onHoldNotificationCallback_client(n);
}


void RobotButterfly::buttonHoldReleasedCallback(uint8_t n) {
  
	if(n == BUTTON_BOTH) {

    if(SERVO_CAL_MODE == true) {
      if(DEBUG_BUTTON_CALLBACKS) Serial << "exiting servo cal mode" << endl;
      SERVO_CAL_MODE = false;
      changeState(RobotButterfly::CURRENT_STATE); // re-enter the state
    } else {
      if(millis() < SERVO_CAL_ENTER_TIME) {
        if(DEBUG_BUTTON_CALLBACKS) Serial << "entering servo cal mode" << endl;
        button_calib_changed = true;
        SERVO_CAL_MODE = true;
      }
    }
    hold_notif_action = false;
  }

  if(onHoldReleasedCallback_client != NULL) onHoldReleasedCallback_client(n);
}


void RobotButterfly::buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT: {
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      if(CHANGE_STATES_CONTROL) incrementState();

      if(SERVO_CAL_MODE) {

      	if(left_cal_dir) {
	        left_cal_mode++;
	      } else {
	        left_cal_mode--;
	      }
	      
	      if(left_cal_mode >= 2) left_cal_dir = !left_cal_dir;
	      if(left_cal_mode <= 0) left_cal_dir = !left_cal_dir;
	      button_calib_changed = true;

	      if(DEBUG_BUTTON_CALLBACKS) Serial << "left: " << left_cal_mode << endl;

      }

    }
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      if(CHANGE_STATES_CONTROL) decrementState();

      if(SERVO_CAL_MODE) {

      	if(right_cal_dir) {
	        right_cal_mode++;
	      } else {
	        right_cal_mode--;
	      }
	      
	      if(right_cal_mode >= 2) right_cal_dir = !right_cal_dir;
	      if(right_cal_mode <= 0) right_cal_dir = !right_cal_dir;
	      button_calib_changed = true;

	      if(DEBUG_BUTTON_CALLBACKS) Serial << "right: " << right_cal_mode << endl;

      }



    break;
  }
  
  if(onClickCallback_client != NULL) onClickCallback_client(n);
}


void RobotButterfly::buttonReleaseCallback(uint8_t n) {
  if(onReleaseCallback_client != NULL) onReleaseCallback_client(n);
}

// ----------------------------------


// ----------------------------------
// --------- sound callbacks --------
// ----------------------------------

SoundCallback RobotButterfly::onSoundDoneCallback_client = NULL;

void RobotButterfly::soundDoneCallback(uint8_t id) {
  
  if(DEBUG_SOUND) Serial << "done sound (" << id << ")" << endl;
  
  if(onSoundDoneCallback_client != NULL) onSoundDoneCallback_client(id);
}

// ----------------------------------


// ----------------------------------
// ---------- imu callbacks ---------
// ----------------------------------

IMUCallback RobotButterfly::onStateChangeCallback_client = NULL;
IMUCallback RobotButterfly::onOrientationChangeCallback_client = NULL;
IMUCallback RobotButterfly::onPoseChangeCallback_client = NULL;
IMUCallback RobotButterfly::onEventDetectedCallback_client = NULL;

void RobotButterfly::imuStateChangeCallback(uint8_t s) {
  
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu state changed" << endl;
  
  if(onStateChangeCallback_client != NULL) onStateChangeCallback_client(s);
}

void RobotButterfly::imuOrientationChangeCallback(uint8_t o) {
  
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu orientation changed" << endl;
  
  if(onOrientationChangeCallback_client != NULL) onOrientationChangeCallback_client(o);
}

void RobotButterfly::imuPoseChangeCallback(uint8_t p) {
  
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu pose changed" << endl;
  
  if(onPoseChangeCallback_client != NULL) onPoseChangeCallback_client(p);
}

void RobotButterfly::imuEventDetectedCallback(uint8_t e) {
  
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu event detected" << endl;
  
  if(onEventDetectedCallback_client != NULL) onEventDetectedCallback_client(e);
}

// ----------------------------------


// ----------------------------------
// -------- neoanim callbacks -------
// ----------------------------------

NeoCallback RobotButterfly::onNeoAnimStartCallback_client;
NeoCallback RobotButterfly::onNeoAnimDoneCallback_client;
NeoCallback RobotButterfly::onNeoAnimLoopCallback_client;

// the neo animation has started
void RobotButterfly::neoAnimStartCallback(struct NeoAnimation *a) {
  
  if(DEBUG_NEOANIM_NEWS) Serial << "Callback: Neo animation (" << a->id << ") start" << endl;
  
  if(onNeoAnimStartCallback_client != NULL) onNeoAnimStartCallback_client(a);
}

// the neo animation is done entirely
void RobotButterfly::neoAnimDoneCallback(struct NeoAnimation *a) {
  
  if(DEBUG_NEOANIM_NEWS) Serial << "Callback: Neo animation (" << a->id << ") done" << endl;

  if(onNeoAnimDoneCallback_client != NULL) onNeoAnimDoneCallback_client(a);
}

// the neo animation is done a loop
void RobotButterfly::neoAnimLoopCallback(struct NeoAnimation *a) {
  
  if(DEBUG_NEOANIM_NEWS) Serial << "Callback: Neo animation (" << a->id << ") looped" << endl;
  
  if(onNeoAnimDoneCallback_client != NULL) onNeoAnimDoneCallback_client(a);
}

// ----------------------------------


// ----------------------------------
// ------ servoanim callbacks -------
// ----------------------------------

ServoAnimCallback RobotButterfly::onServoAnimDoneCallback_client;
ServoAnimCallback RobotButterfly::onServoAnimLoopCallback_client;

// the servo animation is done entirely
void RobotButterfly::servoAnimDoneCallback(struct ServoAnimation *a) {
  
  if(DEBUG_SERVOANIM_NEWS) Serial << "Callback: Servo animation (" << a->id << ") done" << endl;
  
  if(onServoAnimDoneCallback_client != NULL) onServoAnimDoneCallback_client(a);
}

// the servo animation is done a loop
void RobotButterfly::servoAnimLoopCallback(struct ServoAnimation *a) {
  
  if(DEBUG_SERVOANIM_NEWS) Serial << "Callback: Servo animation (" << a->id << ") looped" << endl;

  if(onServoAnimLoopCallback_client != NULL) onServoAnimLoopCallback_client(a);
}

// ----------------------------------


// ----------------------------------
// ------- sensor callbacks ---------
// ----------------------------------

SensorTriggerCallback RobotButterfly::onSensorLightChangeCallback_client;
SensorAmbientCallback RobotButterfly::onSensorLightAmbientChangeCallback_client;
SensorTriggerCallback RobotButterfly::onSensorSoundChangeCallback_client;
SensorAmbientCallback RobotButterfly::onSensorSoundAmbientChangeCallback_client;
SensorTriggerCallback RobotButterfly::onSensorTemperatureChangeCallback_client;
SensorAmbientCallback RobotButterfly::onSensorTemperatureAmbientChangeCallback_client;
SensorTriggerCallback RobotButterfly::onSensorHumidityChangeCallback_client;
SensorAmbientCallback RobotButterfly::onSensorHumidityAmbientChangeCallback_client;

void RobotButterfly::sensorLightChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(onSensorLightChangeCallback_client != NULL) onSensorLightChangeCallback_client(s, trigger_dir);
}

void RobotButterfly::sensorLightAmbientChangeCallback(struct Sensor *s, int change) {
  
	if(onSensorLightAmbientChangeCallback_client != NULL) onSensorLightAmbientChangeCallback_client(s, change);
}

void RobotButterfly::sensorSoundChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(onSensorSoundChangeCallback_client != NULL) onSensorSoundChangeCallback_client(s, trigger_dir);
}

void RobotButterfly::sensorSoundAmbientChangeCallback(struct Sensor *s, int change) {
  
  if(onSensorSoundAmbientChangeCallback_client != NULL) onSensorSoundAmbientChangeCallback_client(s, change);
}

void RobotButterfly::sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(onSensorTemperatureChangeCallback_client != NULL) onSensorTemperatureChangeCallback_client(s, trigger_dir);
}

void RobotButterfly::sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change) {
  
  if(onSensorTemperatureAmbientChangeCallback_client != NULL) onSensorTemperatureAmbientChangeCallback_client(s, change);
}

void RobotButterfly::sensorHumidityChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(onSensorHumidityChangeCallback_client != NULL) onSensorHumidityChangeCallback_client(s, trigger_dir);
}

void RobotButterfly::sensorHumidityAmbientChangeCallback(struct Sensor *s, int change) {
  
  if(onSensorHumidityAmbientChangeCallback_client != NULL) onSensorHumidityAmbientChangeCallback_client(s, change);
}

// ----------------------------------


// ----------------------------------
// ----- proximity callbacks --------
// ----------------------------------

ProximityCallback RobotButterfly::onProximityTriggerCallback_client;

// uses the raw value to trigger the close proximity
// this is called at intervals defined by PROXIMITY_TRIGGER_FREQ
void RobotButterfly::proximityTriggerCallback(struct Proximity *p) {
  
  if(onProximityTriggerCallback_client != NULL) onProximityTriggerCallback_client(p);

}

// ----------------------------------




