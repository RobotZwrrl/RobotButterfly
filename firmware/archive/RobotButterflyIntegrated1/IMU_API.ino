// ------------------------------------
// ---------- IMU Callbacks -----------
// ------------------------------------

void imuStateChangeCallback(uint8_t s) {
  Serial << millis() << " imu state changed" << endl;
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
}

void imuOrientationChangeCallback(uint8_t o) {
  Serial << millis() << " imu orientation changed" << endl;
  // do actions here
  switch(o) {
    case IMU_TABLETOP:
    break;
    case IMU_HANG:
    break;
    case IMU_UNKNOWN:
    break;
  }
}

void imuPoseChangeCallback(uint8_t p) {
  Serial << millis() << " imu pose changed" << endl;
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
}

void imuEventDetectedCallback(uint8_t e) {
  Serial << millis() << " imu event detected" << endl;
  // do actions here
  if(e) {
    // event detected
  }
}

// ------------------------------------


// ------------------------------------
// ----------- IMU Getters ------------
// ------------------------------------

// returns IMU_STATE which is an enum: IMUStates
uint8_t getIMUState() {
  return IMU_STATE;
}

// returns IMU_ORIENTATION which is an enum: IMUOrientations
uint8_t getIMUOrientation() {
  return IMU_ORIENTATION;
}

// returns IMU_POSE which is an enum: IMUPoses
uint8_t getIMUPose() {
  return IMU_POSE;
}

// ------------------------------------

