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
  switch(n) {
    case BUTTON_BOTH:
    break;
    case BUTTON_LEFT:
    break;
    case BUTTON_RIGHT:
    break;
  }
  
  if(onHoldReleasedCallback_client != NULL) onHoldReleasedCallback_client(n);
}

// do an action here
void RobotButterfly::buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      incrementState();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      decrementState();
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
  
  if(onStateChangeCallback_client != NULL) onStateChangeCallback_client(s);
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
  
  if(onOrientationChangeCallback_client != NULL) onOrientationChangeCallback_client(o);
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
  
  if(onPoseChangeCallback_client != NULL) onPoseChangeCallback_client(p);
}

void RobotButterfly::imuEventDetectedCallback(uint8_t e) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu event detected" << endl;
  // do actions here
  if(e) {
    // event detected
  }
  
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
  //if(a->type == NEO_ANIM_HOME) return; // sometimes the home animations can be annoying lol
  if(DEBUG_NEOANIM_NEWS) Serial << "Callback: Neo animation (" << a->id << ") done" << endl;
  
  if(onNeoAnimDoneCallback_client != NULL) onNeoAnimDoneCallback_client(a);
}

// the neo animation is done a loop
void RobotButterfly::neoAnimLoopCallback(struct NeoAnimation *a) {
  
  //if(a->type == NEO_ANIM_HOME) return; // sometimes the home animations can be annoying lol
  if(DEBUG_NEOANIM_NEWS) Serial << "Callback: Neo animation (" << a->id << ") looped" << endl;
  
  if(onNeoAnimDoneCallback_client != NULL) onNeoAnimDoneCallback_client(a);
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




