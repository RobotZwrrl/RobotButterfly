void imuPoseChangeCallback(uint8_t p) {
  if(p == IMU_Pose_Home) {
    homePosAction();
  }
}


void updateMovement() {

  if(getIMUState() != IMU_ACTIVE) return;

  if(millis()-last_pose_movement <= 2000) return; 

  uint8_t p = getIMUPose();

  home_pos_active = false;
  last_pose_movement = millis();

  switch(p) {
    case IMU_Pose_Tilt_L: 
      tiltLeftAction();
    break;
    case IMU_Pose_Tilt_R: 
      tiltRightAction();
    break;
    case IMU_Pose_Tilt_Fwd:
      tiltFwdAction();
    break;
    case IMU_Pose_Tilt_Bwd:
      tiltBwdAction();
    break;
  }

}

