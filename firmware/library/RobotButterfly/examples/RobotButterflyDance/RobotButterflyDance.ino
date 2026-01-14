/* Robot Butterfly Dance
 * --------------------------
 * Respond to the IMU poses with different
 * light patterns and servo animations!
 * when at rest, flap the wings every 20s.
 */

#include <RobotButterfly.h>

RobotButterfly robotbutterfly;

void imuPoseChangeCallback(uint8_t p);

long last_movement_rest = 0;
long last_pose_movement = 0;


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  
  robotbutterfly.init(true, false);

  robotbutterfly.onPoseChangeCallback_client = imuPoseChangeCallback;

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  setTaskPriorities();

  Serial << "Welcome to Robot Butterfly!" << endl;
  playSound(SOUND_ALERT_STARTUP);

}

void loop() {
  
  robotbutterfly.update();

  updateMovement();

  if(getIMUState() != IMU_ACTIVE) {
    digitalWrite(LED_HEARTBEAT_PIN, LOW);
  } else {
    digitalWrite(LED_HEARTBEAT_PIN, HIGH);
  }

  if(getIMUPose() == IMU_Pose_Home && getIMUState() == IMU_ACTIVE) {
    
    if(millis()-last_movement_rest >= 20000 && 
       millis() > 10000 &&
       millis()-last_pose_movement >= 5000) {
      
      setServoAnim(&servo_animation_alert, SERVO_ANIM_FLUTTER, SERVO_ANIM_ALERT);
      setServoAnimFlutterWings(&servo_animation_alert, SERVO_ANIM_FLUTTER_WINGS_BOTH_UP);
      setServoAnimDuration(&servo_animation_alert, 3000);
      startServoAnim(&servo_animation_alert);
      last_movement_rest = millis();

    }
  }

}


void setTaskPriorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_LOW);
  setIMUTaskPriority(PRIORITY_IMU_MID);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

