void imuPoseChangeCallback(uint8_t p) {
  if(p == IMU_Pose_Home) {
    homePosAction();
  }
}


void updateMovement() {

  if(getIMUState() != IMU_ACTIVE) return;

  if(millis()-last_pose_movement <= 2000) return; 

  uint8_t p = getIMUPose();

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


void tiltLeftAction() {

  Serial << "tilt left!" << endl;

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_LEFT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_WHITE);
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_WILDFLOWER);

  last_pose_movement = millis();
}


void tiltRightAction() {

  Serial << "tilt right!" << endl;

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME);
  setServoAnimFlutterPos(&servo_animation_alert, SERVO_ANIM_FLUTTER_POS_UP);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_WHITE);
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_DAISY);

  last_pose_movement = millis();
}


void tiltFwdAction() {
  Serial << "tilt fwd!" << endl;

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_DOWN);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_alert, NEO_LAVENDER, NEO_WHITE);
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_LAVENDER);

  last_pose_movement = millis();
}


void tiltBwdAction() {
  Serial << "tilt bwd!" << endl;

  setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
  setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME);
  setServoAnimSpeed(&servo_animation_alert, 200);
  setServoAnimDuration(&servo_animation_alert, 2000);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_SPRINKLE, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_alert, NEO_GOLDEN_YELLOW, NEO_WHITE);
  setNeoAnimSpeed(&neo_animation_alert, 100);
  setNeoAnimDuration(&neo_animation_alert, 2000);
  startNeoAnim(&neo_animation_alert);

  playSound(SOUND_SMELL_SUNFLOWER);

  last_pose_movement = millis();
}


void homePosAction() {
  if(millis()-last_movement_rest <= 3000) return;

  setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
  setServoAnimPositionLeft(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
  setServoAnimPositionRight(&servo_animation_alert, SERVO_ANIM_POSITION_UP);
  setServoAnimSpeed(&servo_animation_alert, 100);
  startServoAnim(&servo_animation_alert);

  setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
  setNeoAnimDuration(&neo_animation_alert, 100);
  startNeoAnim(&neo_animation_alert);
}

