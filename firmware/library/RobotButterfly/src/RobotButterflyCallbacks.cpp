#include "RobotButterfly.h"

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

  if(onHoldNotificationCallback_client != NULL) onHoldNotificationCallback_client(n);
}

// do an action here
void RobotButterfly::buttonHoldReleasedCallback(uint8_t n) {
  
  if(onHoldReleasedCallback_client != NULL) onHoldReleasedCallback_client(n);
}

// do an action here
void RobotButterfly::buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      if(CHANGE_STATES_CONTROL) incrementState();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      if(CHANGE_STATES_CONTROL) decrementState();
    break;
  }
  
  if(onClickCallback_client != NULL) onClickCallback_client(n);
}

// probably not necessary to do anything here
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

NeoCallback RobotButterfly::onNeoAnimDoneCallback_client;
NeoCallback RobotButterfly::onNeoAnimLoopCallback_client;

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




