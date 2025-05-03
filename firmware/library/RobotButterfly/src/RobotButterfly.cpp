#include "RobotButterfly.h"

RobotButterfly::RobotButterfly() {
  onHoldNotificationCallback = NULL;
  onHoldReleasedCallback = NULL;
  onClickCallback = NULL;
  onReleaseCallback = NULL;
  
  onSoundDoneCallback = NULL;

  onStateChangeCallback = NULL;
  onOrientationChangeCallback = NULL;
  onPoseChangeCallback = NULL;
  onEventDetectedCallback = NULL;

  onNeoAnimDoneCallback = NULL;
  onNeoAnimLoopCallback = NULL;

  onServoAnimDoneCallback = NULL;
  onServoAnimLoopCallback = NULL;
}

void RobotButterfly::init() {
  initButtons();
  initSound();
  initIMU();
  initNeopixels();
  initNeoAnimations();
  initServos(SERVO_MODE_INIT_BOTH);
  initServoAnimations();

  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_MAGENTA, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);

  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // startServoAnim(&servo_animation_alert);

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

}

void RobotButterfly::update() {
    
    updateButtons();
    //updateIMU();
    updateNeoAnimation();
    updateServoAnimation();

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


