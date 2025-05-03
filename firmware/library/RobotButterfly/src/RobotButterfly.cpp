#include "RobotButterfly.h"

RobotButterfly::RobotButterfly() {
}

void RobotButterfly::init() {
  initButtons();
  initSound();
  // initIMU();
  // initNeopixels();
  // initNeoAnimations();
  // initServos(SERVO_MODE_INIT_BOTH);
  // initServoAnimations();
  // initSensors();
  initProximity();


  // -- button callbacks --
  onHoldNotificationCallback = buttonHoldNotificationCallback;
  onHoldReleasedCallback = buttonHoldReleasedCallback;
  onClickCallback = buttonClickCallback;
  onReleaseCallback = buttonReleaseCallback;
  // --

  // -- sound callbacks --
  onSoundDoneCallback = soundDoneCallback;
  // --

  // -- imu callbacks --
  onStateChangeCallback = imuStateChangeCallback;
  onOrientationChangeCallback = imuOrientationChangeCallback;
  onPoseChangeCallback = imuPoseChangeCallback;
  onEventDetectedCallback = imuEventDetectedCallback;
  // --

  // -- neoanim callbacks --
  onNeoAnimDoneCallback = neoAnimDoneCallback;
  onNeoAnimLoopCallback = neoAnimLoopCallback;
  // --

  // -- sensor callbacks --
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorChangeCallback = sensorHumidityChangeCallback;
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorAmbientChangeCallback = sensorHumidityAmbientChangeCallback;

  all_sensors[SENSOR_ID_LIGHT]->onSensorChangeCallback = sensorLightChangeCallback;
  all_sensors[SENSOR_ID_LIGHT]->onSensorAmbientChangeCallback = sensorLightAmbientChangeCallback;

  all_sensors[SENSOR_ID_SOUND]->onSensorChangeCallback = sensorSoundChangeCallback;
  all_sensors[SENSOR_ID_SOUND]->onSensorAmbientChangeCallback = sensorSoundAmbientChangeCallback;

  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorChangeCallback = sensorTemperatureChangeCallback;
  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorAmbientChangeCallback = sensorTemperatureAmbientChangeCallback;
  // --

  // -- proximity callbacks --
  ultrasonic.onProximityTriggerCallback = proximityTriggerCallback;
  // --

  // neoanim testing
  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_MAGENTA, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);

  // servoanim testing
  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // startServoAnim(&servo_animation_alert);

  // sensor testing
  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    if(all_sensors[i]) all_sensors[i]->print = true;
  }

}


void RobotButterfly::update() {

    updateButtons();
    // updateIMU();
    // updateNeoAnimation();
    // updateServoAnimation();
    // updateSensors();
    updateProximity();

}



// ----------------------------------
// --------- button callbacks -------
// ----------------------------------

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
  // TODO: call user callback
}

// do an action here
void RobotButterfly::buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
    break;
    case BUTTON_LEFT:
    break;
    case BUTTON_RIGHT:
    break;
  }
  // TODO: call user callback
}

// do an action here
void RobotButterfly::buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
    break;
  }
  // TODO: call user callback
}

// probably not necessary to do anything here
void RobotButterfly::buttonReleaseCallback(uint8_t n) {
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// --------- sound callbacks --------
// ----------------------------------

void RobotButterfly::soundDoneCallback(uint8_t id) {
  if(DEBUG_SOUND) Serial << "done sound (" << id << ")" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ---------- imu callbacks ---------
// ----------------------------------

void RobotButterfly::imuStateChangeCallback(uint8_t s) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu state changed" << endl;
  // do actions here
  switch(s) {
    case IMU_SETTLE:
    break;
    case IMU_CALIBRATE_HOME:
    break;
    case IMU_ACTIVE:
    break;
    case IMU_INACTIVE:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuOrientationChangeCallback(uint8_t o) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu orientation changed" << endl;
  // do actions here
  switch(o) {
    case IMU_TABLETOP:
    break;
    case IMU_HANG:
    break;
    case IMU_UNKNOWN:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuPoseChangeCallback(uint8_t p) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu pose changed" << endl;
  // do actions here
  switch(p) {
    case IMU_Pose_Tilt_L:
    break;
    case IMU_Pose_Tilt_R:
    break;
    case IMU_Pose_Tilt_Fwd:
    break;
    case IMU_Pose_Tilt_Bwd:
    break;
    case IMU_Pose_Home:
    break;
    case IMU_Pose_NA:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuEventDetectedCallback(uint8_t e) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu event detected" << endl;
  // do actions here
  if(e) {
    // event detected
  }
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// -------- neoanim callbacks -------
// ----------------------------------

// the neo animation is done entirely
void RobotButterfly::neoAnimDoneCallback(struct NeoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") done" << endl;
  // TODO: call user callback
}

// the neo animation is done a loop
void RobotButterfly::neoAnimLoopCallback(struct NeoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") looped" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ------ servoanim callbacks -------
// ----------------------------------

// the servo animation is done
void RobotButterfly::servoAnimDoneCallback(struct ServoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == SERVO_ANIM_HOME) return;
  Serial << "Callback: Servo animation (" << a->id << ") done" << endl;
  // TODO: call user callback
}

// the servo animation has looped
void RobotButterfly::servoAnimLoopCallback(struct ServoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == SERVO_ANIM_HOME) return;
  if(a->num_frames <= 1) return; // skip the one frame animations
  Serial << "Callback: Servo animation (" << a->id << ") looped" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ------- sensor callbacks ---------
// ----------------------------------

void RobotButterfly::sensorLightChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Light on!";
  } else {
    Serial << "---> Light off!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorLightAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Light ambient brighter!" << endl;
  } else {
    Serial << "---> Light ambient darker!" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorSoundChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Sound louder!";
  } else {
    Serial << "---> Sound quieter!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorSoundAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Sound ambient louder!" << endl;
  } else {
    Serial << "---> Sound ambient quieter!" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Temperature warmer!";
  } else {
    Serial << "---> Temperature colder!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Temperature ambient warmer" << endl;
  } else {
    Serial << "---> Temperature ambient colder" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorHumidityChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == true) {
    Serial << "---> Humidity decrease!";
  } else {
    Serial << "---> Humidity increase!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorHumidityAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change > 0) {
    Serial << "---> Humidity ambient decrease" << endl;
  } else {
    Serial << "---> Humidity ambient increase" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

// ----------------------------------


// ----------------------------------
// ----- proximity callbacks --------
// ----------------------------------

// uses the raw value to trigger the close proximity
// this is called at intervals defined by PROXIMITY_TRIGGER_FREQ
void RobotButterfly::proximityTriggerCallback(struct Proximity *p) {
  
  // TODO: remember to get rid of the lines below
  
  Serial << "----> Close proximity detected!" << endl;
  
  // TODO: call user callback

}

// ----------------------------------


