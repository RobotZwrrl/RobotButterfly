#include "modules/IMU/IMU.h"

IMUCallback onStateChangeCallback = NULL;
IMUCallback onOrientationChangeCallback = NULL;
IMUCallback onPoseChangeCallback = NULL;
IMUCallback onEventDetectedCallback = NULL;

bool checkOrientationIMU();
bool checkPositionIMU();
bool checkEventIMU();

void checkRecalibrateIMUHome();

void printIMUStats();
void printIMURaw();
void printIMUHome();
void printIMUDataAvg();
void printIMUDeltaHomeAvg();

void imuDeltaCalculations();
void imuResetMovingAverages();
void imuUpdateAvgValues();


// ------------- imu isr --------------
void IRAM_ATTR Timer_10Hz_imu_ISR() { // every 0.1 seconds
  new_avg_sample = true;
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


// ------------------------------------
// ----------- IMU Internal -----------
// ------------------------------------

void updateIMU() {

  if(IMU_STATE == IMU_INACTIVE) return;

  if(IMU_STATE_PREV != IMU_STATE) {
    if(onStateChangeCallback) onStateChangeCallback(IMU_STATE);
  }

  // vars for enter states
  if(IMU_STATE == IMU_SETTLE && IMU_STATE_PREV != IMU_STATE) {
    settle_start = millis();
  }
  IMU_STATE_PREV = IMU_STATE;

  // store the previous values
  imu_prev.ax = imu.ax;
  imu_prev.ay = imu.ay;
  imu_prev.az = imu.az;
  imu_prev.gx = imu.gx;
  imu_prev.gy = imu.gy;
  imu_prev.gz = imu.gz;
  imu_prev.last_data = imu.last_data;

  // get the new values
  mpu.getMotion6(&imu.ax, &imu.ay, &imu.az, &imu.gx, &imu.gy, &imu.gz);
  imu.last_data = millis();

  // check for settle down state (prior to calibration)
  if(IMU_STATE == IMU_SETTLE) {

    // prints...
    if(DEBUG_IMU) {
      if(millis()-last_imu_calib_print >= 100) {
        Serial << "waiting for the robot to settle " << millis()-settle_start << " / " << IMU_HOME_SETTLE_TIME << endl;
        last_imu_calib_print = millis();
      }
    }

    // wait for it to settle on startup
    // (eg, after pressing the on/off switch)
    if(millis()-settle_start >= IMU_HOME_SETTLE_TIME) {
      
      if(DEBUG_IMU) Serial << "IMU_SETTLE done, sending to IMU_CALIBRATE_HOME" << endl;

      // clear it out before sending to calibration
      imu_avg_data_ax.reset();
      imu_avg_data_ay.reset();
      imu_avg_data_az.reset();
      imu_avg_data_gx.reset();
      imu_avg_data_gy.reset();
      imu_avg_data_gz.reset();
      
      IMU_STATE = IMU_CALIBRATE_HOME;
      calibration_start = millis();
      return;
    }

    return;
  }

  // calibrate the home position
  if(IMU_STATE == IMU_CALIBRATE_HOME) {

    imu_avg_data_ax.reading(imu.ax);
    imu_avg_data_ay.reading(imu.ay);
    imu_avg_data_az.reading(imu.az);
    imu_avg_data_gx.reading(imu.gx);
    imu_avg_data_gy.reading(imu.gy);
    imu_avg_data_gz.reading(imu.gz);

    if(DEBUG_IMU) {
      if(millis()-last_imu_calib_print >= 100) {
        Serial << "calibrating... " << millis()-calibration_start << " / " << IMU_HOME_CALIBRATION_TIME << endl;
        last_imu_calib_print = millis();
      }
    }

    // calibration_start is the start time of the calibration
    if(millis()-calibration_start >= IMU_HOME_CALIBRATION_TIME) {

      // set the home values
      imu_home.ax = imu_avg_data_ax.getAvg();
      imu_home.ay = imu_avg_data_ay.getAvg();
      imu_home.az = imu_avg_data_az.getAvg();
      imu_home.gx = imu_avg_data_gx.getAvg();
      imu_home.gy = imu_avg_data_gy.getAvg();
      imu_home.gz = imu_avg_data_gz.getAvg();
      imu_home.last_data = millis();

      // reset now that it's done
      calibration_start = 0;
      last_home_calibration = millis();

      if(DEBUG_IMU) Serial << "calibration done, sending to IMU_ACTIVE" << endl;

      IMU_STATE = IMU_ACTIVE;
      return;
    }

    return;
  }

  // ------- IMU_ACTIVE STATE -------

  // update the moving average filter with the
  // calculated deltas
  imuDeltaCalculations();

  // get the average after 0.5 seconds, the
  // hardware timer interrupt sets this flag
  if(new_avg_sample) {

    // get the values into their structs
    imuUpdateAvgValues();

    // check what orientation it is in
    bool o = checkOrientationIMU();

    // check if there's a new position only in the 
    // tabletop orientation. pose check implements
    // a lockout time inside of the function, and the
    // returned bool reflects that.
    bool p = false;
    if(IMU_ORIENTATION != IMU_HANG) {
      p = checkPositionIMU();
    }

    // check if there's a new event
    bool e = checkEventIMU();

    // continuously check if home needs to be recalibrated
    // regardless of orientation, but not if
    // there's a pose or event
    if(PREFS_IMU_AUTO_RECALIBRATE_HOME) {
      if(p == false && e == false) {
        if(DEBUG_IMU) Serial << "checking to recalibrate home" << endl;
        checkRecalibrateIMUHome();
      }
    }

    // prints
    if(IMU_PRINT_DATA_AVG) printIMUDataAvg();
    if(IMU_PRINT_DELTA_HOME_AVG) printIMUDeltaHomeAvg();
    if(IMU_PRINT_STATS) printIMUStats();

    // get ready for the next sample by resetting
    // the moving average filter and resetting the flag
    imuResetMovingAverages();
    new_avg_sample = false;
    
  }

  // prints at realtime
  if(IMU_PRINT_RAW) printIMURaw();

}


void initIMU() {

  // --
  new_avg_sample = false;
  timer_10Hz_imu_cfg = NULL;

  IMU_STATE = IMU_INACTIVE;
  IMU_STATE_PREV = IMU_INACTIVE;

  IMU_ORIENTATION = IMU_UNKNOWN;
  IMU_ORIENTATION_PREV = IMU_UNKNOWN;
  last_orientation_change = 0;

  IMU_POSE = IMU_Pose_NA;
  IMU_POSE_PREV = IMU_Pose_NA;
  last_pose_detected = 0;

  EVENT_DETECTED = false;
  last_event_detected = 0;
  last_event_score_clear = 0;
  event_score = 0;

  last_imu_calib_print = 0;
  last_home_calibration = 0;
  settle_start = 0;
  calibration_start = 0;
  last_score_clear = 0;
  home_recalibrate_score = 0;
  last_imu_stats_print = 0;

  IMU_PRINT_RAW = false;
  IMU_PRINT_DATA_AVG = false;
  IMU_PRINT_DELTA_HOME_AVG = false; // usually true during dev
  IMU_PRINT_DELTA_TIME_AVG = false;
  IMU_PRINT_STATS = true; // usually true during testing
  // --

  Wire.begin();

  Serial << "Initialising MPU...";
  mpu.initialize();

  Serial << "Testing MPU6050 connection...";
  if(mpu.testConnection() ==  false){
    Serial << "MPU6050 connection failed";
    IMU_STATE = IMU_INACTIVE;
  } else {
    Serial << "MPU6050 connection successful" << endl;
    IMU_STATE = IMU_ACTIVE;
  }

  if(IMU_STATE == IMU_INACTIVE) return;

  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_imu_cfg = timerBegin(2, 16000, true);
  timerAttachInterrupt(timer_10Hz_imu_cfg, &Timer_10Hz_imu_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_imu_cfg, 500, true); // 10 Hz
  timerAlarmEnable(timer_10Hz_imu_cfg);

  imu_home.ax = 0;
  imu_home.ay = 0;
  imu_home.az = 0;
  imu_home.gx = 0;
  imu_home.gy = 0;
  imu_home.gz = 0;
  imu_home.last_data = 0;

  imu_avg_data_ax.begin();
  imu_avg_data_ay.begin();
  imu_avg_data_az.begin();
  imu_avg_data_gx.begin();
  imu_avg_data_gy.begin();
  imu_avg_data_gz.begin();

  imu_avg_home_ax.begin();
  imu_avg_home_ay.begin();
  imu_avg_home_az.begin();
  imu_avg_home_gx.begin();
  imu_avg_home_gy.begin();
  imu_avg_home_gz.begin();

  IMU_STATE = IMU_SETTLE;

  if(RTOS_ENABLED) { 

    Mutex_IMU = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_IMU_code,     // task function
                      "Task_IMU",        // name of task
                      STACK_IMU,         // stack size of task
                      NULL,                  // parameter of the task
                      PRIORITY_IMU_MID,  // priority of the task (low number = low priority)
                      &Task_IMU,         // task handle to keep track of created task
                      TASK_CORE_IMU);    // pin task to core
  }

}


bool checkOrientationIMU() {
  
  IMU_ORIENTATION_PREV = IMU_ORIENTATION;

  bool orientation_detected = false;

  // tabletop
  if(abs(imu_avg.ax) <= 1000 && abs(imu_avg.ay) <= 1000 && 
     abs(imu_avg.az) >= 6000 && imu_avg.az < 0) {
      if(DEBUG_IMU) Serial << "orientation: tabletop" << endl;
      IMU_ORIENTATION = IMU_TABLETOP;
      orientation_detected = true;
  }

  // hang
  if(abs(imu_avg.ax) <= 1000 && abs(imu_avg.ay) >= 6000 && 
     abs(imu_avg.az) <= 3000 && imu_avg.ay > 0) {
      if(DEBUG_IMU) Serial << "orientation: hang" << endl;
      IMU_ORIENTATION = IMU_HANG;
      orientation_detected = true;
  }

  if(IMU_ORIENTATION_PREV != IMU_ORIENTATION) {
    last_orientation_change = millis();
    if(onOrientationChangeCallback) onOrientationChangeCallback(IMU_ORIENTATION);
  }

  if(orientation_detected) {

    if(IMU_ORIENTATION_PREV != IMU_ORIENTATION) {
      if(DEBUG_IMU) Serial << "orientation changed!" << endl;
    }

    return true;

  } else {
    
    if(millis()-last_orientation_change >= IMU_ORIENTATION_CHANGE_LOCKOUT) {
      if(DEBUG_IMU) Serial << "orientation unknown" << endl;
      IMU_ORIENTATION = IMU_UNKNOWN;
    }

  }

  return false;
}


bool checkPositionIMU() {

  bool pose_detected = false;

  if(IMU_POSE_PREV != IMU_POSE) {
    if(onPoseChangeCallback) onPoseChangeCallback(IMU_POSE);
  }
  IMU_POSE_PREV = IMU_POSE;

  // left / right tilt
  if(!pose_detected) {
    if(abs(imu_delta_home_avg.ax) <= 8000 && abs(imu_delta_home_avg.ax) >= 2300) {
      if(abs(imu_delta_home_avg.ay) <= 1000) {
        if(abs(imu_delta_home_avg.az) >= 300) {
          if(DEBUG_IMU) Serial << "pose detected: ";
          if(imu_delta_home_avg.ax < 0) {
            if(DEBUG_IMU) Serial << "tilt right" << endl;
            IMU_POSE = IMU_Pose_Tilt_R;
          } else {
            if(DEBUG_IMU) Serial << "tilt left" << endl;
            IMU_POSE = IMU_Pose_Tilt_L;
          }
          pose_detected = true;
        }
      }
    }
  }

  // front / back tilt
  if(!pose_detected) {
    if(abs(imu_delta_home_avg.ay) <= 8000 && abs(imu_delta_home_avg.ay) >= 2100) {
      if(abs(imu_delta_home_avg.ax) <= 1500) {
        if(DEBUG_IMU) Serial << "pose detected: ";
        if(imu_delta_home_avg.ay < 0) {
          if(DEBUG_IMU) Serial << "tilt backwards" << endl;
          IMU_POSE = IMU_Pose_Tilt_Bwd;
        } else {
          if(DEBUG_IMU) Serial << "tilt forwards" << endl;
          IMU_POSE = IMU_Pose_Tilt_Fwd;
        }
        pose_detected = true;
      }
    }
  }

  if(pose_detected) {
    home_recalibrate_score = 0; // reset the home recal score for after the lockout
    last_pose_detected = millis();
  }

  if(!pose_detected) {
    if(millis()-last_pose_detected <= IMU_POSE_LOCKOUT) {
      return true; // force true as it is still in lockout period
    } else {
      IMU_POSE = IMU_Pose_NA;
    }
  }

  return pose_detected;
}


bool checkEventIMU() {

  // make sure it doesn't loop detecting events
  if(millis()-last_event_detected < IMU_EVENT_LOCKOUT_TIME) return true;

  int delta[3];

  delta[0] = abs(imu_delta_home_avg.gx);
  delta[1] = abs(imu_delta_home_avg.gy);
  delta[2] = abs(imu_delta_home_avg.gz);

  for(uint8_t i=0; i<3; i++) {
    if(IMU_ORIENTATION == IMU_TABLETOP) {
      if(delta[i] >= IMU_DELTA_EVENT_THRESH_TABLETOP) {
        event_score++;
      }
    } else if(IMU_ORIENTATION == IMU_HANG) {
      if(delta[i] >= IMU_DELTA_EVENT_THRESH_HANG) {
        event_score++;
      }
    } else {
      if(delta[i] >= IMU_DELTA_EVENT_THRESH_HANG) {
        event_score++;
      }
    }
  }
  if(DEBUG_IMU) Serial << "event score: " << event_score << endl;

  // toggle that it is an event
  if(event_score >= IMU_EVENT_SCORE_THRESH) {
    if(DEBUG_IMU) Serial << "event detected, score: " << event_score << endl;
    EVENT_DETECTED = true;
    last_event_detected = millis();
    event_score = 0;
    last_event_score_clear = millis();
    if(onEventDetectedCallback) onEventDetectedCallback(EVENT_DETECTED);
    return true;
  }

  // clear the score every so often
  // the score accumulates - so that instantaneous
  // anomolies don't trigger an event
  if(millis()-last_event_score_clear >= IMU_EVENT_SCORE_CLEAR) {
    EVENT_DETECTED = false;
    event_score -= 1;
    last_event_score_clear = millis();
    if(event_score < 0) event_score = 0;
    //if(DEBUG_IMU) Serial << "decremented event score: " << event_score << endl;
  }
  
  return false;
}


void checkRecalibrateIMUHome() {

  // make sure it doesn't loop recalibrating
  if(millis()-last_home_calibration < IMU_HOME_RECALIBRATION_LOCKOUT_TIME) return;

  int delta[6];

  delta[0] = abs(imu_delta_home_avg.ax);
  delta[1] = abs(imu_delta_home_avg.ay);
  delta[2] = abs(imu_delta_home_avg.az);
  delta[3] = abs(imu_delta_home_avg.gx);
  delta[4] = abs(imu_delta_home_avg.gy);
  delta[5] = abs(imu_delta_home_avg.gz);

  for(uint8_t i=0; i<6; i++) {
    if(delta[i] >= IMU_DELTA_RECALIBRATE_HOME_THRESH) {
      home_recalibrate_score++;
    }
  }
  if(DEBUG_IMU) Serial << "score: " << home_recalibrate_score << endl;

  // toggle that it needs to be recalibrated
  if(home_recalibrate_score >= IMU_HOME_SCORE_THRESH) {
    if(DEBUG_IMU) Serial << "home recalibration needed, score: " << home_recalibrate_score << endl;
    home_recalibrate_score = 0;
    last_score_clear = millis();

    // skip the settle if it's in hang orientation
    // as it is very likely to never be 'settled' due
    // to wind etc
    if(IMU_ORIENTATION == IMU_HANG) {
      calibration_start = millis();
      IMU_STATE = IMU_CALIBRATE_HOME;
      return;
    }

    IMU_STATE = IMU_SETTLE;
    return;
  }

  // clear the score every so often
  // the score accumulates - so that instantaneous
  // anomolies don't trigger a recalibration
  if(millis()-last_score_clear >= IMU_HOME_RECAL_SCORE_CLEAR) {
    home_recalibrate_score -= 3;
    last_score_clear = millis();
    if(home_recalibrate_score < 0) home_recalibrate_score = 0;
    if(DEBUG_IMU) Serial << "decremented score: " << home_recalibrate_score << endl;
  }

}


void printIMUStats() {

  if(!DEBUG_IMU_NEWS) return;

  if(millis()-last_imu_stats_print < 500) return;

  last_imu_stats_print = millis();

  String readable_state = "";
  switch(IMU_STATE) {
    case IMU_SETTLE:
      readable_state = "SETTLE";
    break;
    case IMU_CALIBRATE_HOME:
      readable_state = "CALIBRATE_HOME";
    break;
    case IMU_ACTIVE:
      readable_state = "ACTIVE";
    break;
    case IMU_INACTIVE:
      readable_state = "INACTIVE";
    break;
  }

  String readable_orientation = "";
  switch(IMU_ORIENTATION) {
    case IMU_TABLETOP:
      readable_orientation = "TABLETOP";
    break;
    case IMU_HANG:
      readable_orientation = "HANG";
    break;
    case IMU_UNKNOWN:
      readable_orientation = "UNKNOWN";
    break;
  }

  String readable_pose = "";
  switch(IMU_POSE) {
    case IMU_Pose_Tilt_L:
      readable_pose = "TILT LEFT";
    break;
    case IMU_Pose_Tilt_R:
      readable_pose = "TILT RIGHT";
    break;
    case IMU_Pose_Tilt_Fwd:
      readable_pose = "TILT FWD";
    break;
    case IMU_Pose_Tilt_Bwd:
      readable_pose = "TILT BWD";
    break;
    case IMU_Pose_Home:
      readable_pose = "HOME";
    break;
    case IMU_Pose_NA:
      readable_pose = "N/A";
    break;
  }

  String readable_event = "";
  if(EVENT_DETECTED) {
    readable_event = "!!!! EVENT DETECTED";
  } else {
    readable_event = "NO EVENT";
  }

  String recalibrate_pref = "";
  if(PREFS_IMU_AUTO_RECALIBRATE_HOME) {
    recalibrate_pref = "TRUE";
  } else {
    recalibrate_pref = "FALSE";
  }

  Serial << "------------- " << millis() << " ------------" << endl;
  Serial << "State: " << readable_state << endl;
  Serial << "Orientation: " << readable_orientation << endl;
  Serial << "Pose: " << readable_pose << endl;
  Serial << "Event: " << readable_event << " (" << event_score << ")" << endl;
  Serial << "Recalibrate: " << recalibrate_pref << " (" << home_recalibrate_score << ")" << endl;
  Serial << "--------------------------------" << endl;

}

void printIMURaw() {
  Serial.print("a/g:\t");
  Serial.print(imu.ax); Serial.print("\t");
  Serial.print(imu.ay); Serial.print("\t");
  Serial.print(imu.az); Serial.print("\t");
  Serial.print(imu.gx); Serial.print("\t");
  Serial.print(imu.gy); Serial.print("\t");
  Serial.println(imu.gz);

  // Serial.print("a/g:\t");
  // Serial.print(imu.ax * mpu.get_acce_resolution(), 3); Serial.print("\t");
  // Serial.print(imu.ay * mpu.get_acce_resolution(), 3); Serial.print("\t");
  // Serial.print(imu.az * mpu.get_acce_resolution(), 3); Serial.print("\t");
  // Serial.print(imu.gx * mpu.get_gyro_resolution(), 3); Serial.print("\t");
  // Serial.print(imu.gy * mpu.get_gyro_resolution(), 3); Serial.print("\t");
  // Serial.println(imu.gz * mpu.get_gyro_resolution(), 3);
}

void printIMUHome() {
  Serial.print("a/g:\t");
  Serial.print(imu_home.ax); Serial.print("\t");
  Serial.print(imu_home.ay); Serial.print("\t");
  Serial.print(imu_home.az); Serial.print("\t");
  Serial.print(imu_home.gx); Serial.print("\t");
  Serial.print(imu_home.gy); Serial.print("\t");
  Serial.println(imu_home.gz);
}

void printIMUDataAvg() {
  Serial.print("a/g:\t");
  Serial.print(imu_avg.ax); Serial.print("\t");
  Serial.print(imu_avg.ay); Serial.print("\t");
  Serial.print(imu_avg.az); Serial.print("\t");
  Serial.print(imu_avg.gx); Serial.print("\t");
  Serial.print(imu_avg.gy); Serial.print("\t");
  Serial.println(imu_avg.gz);
}

void printIMUDeltaHomeAvg() {
  Serial.print("a/g:\t");
  Serial.print(imu_delta_home_avg.ax); Serial.print("\t");
  Serial.print(imu_delta_home_avg.ay); Serial.print("\t");
  Serial.print(imu_delta_home_avg.az); Serial.print("\t");
  Serial.print(imu_delta_home_avg.gx); Serial.print("\t");
  Serial.print(imu_delta_home_avg.gy); Serial.print("\t");
  Serial.println(imu_delta_home_avg.gz);
}

void imuDeltaCalculations() {
  
  // add the raw data to its moving average filter
  imu_avg_data_ax.reading(imu.ax);
  imu_avg_data_ay.reading(imu.ay);
  imu_avg_data_az.reading(imu.az);
  imu_avg_data_gx.reading(imu.gx);
  imu_avg_data_gy.reading(imu.gy);
  imu_avg_data_gz.reading(imu.gz);

  // calculate the delta relative to home position
  int delta_home_ax = imu.ax-imu_home.ax;
  int delta_home_ay = imu.ay-imu_home.ay;
  int delta_home_az = imu.az-imu_home.az;
  int delta_home_gx = imu.gx-imu_home.gx;
  int delta_home_gy = imu.gy-imu_home.gy;
  int delta_home_gz = imu.gz-imu_home.gz;

  // calculate the delta relative to the previous reading
  int delta_time_ax = imu.ax-imu_prev.ax;
  int delta_time_ay = imu.ay-imu_prev.ay;
  int delta_time_az = imu.az-imu_prev.az;
  int delta_time_gx = imu.gx-imu_prev.gx;
  int delta_time_gy = imu.gy-imu_prev.gy;
  int delta_time_gz = imu.gz-imu_prev.gz;

  // put the home delta into the moving average filter
  imu_avg_home_ax.reading(delta_home_ax);
  imu_avg_home_ay.reading(delta_home_ay);
  imu_avg_home_az.reading(delta_home_az);
  imu_avg_home_gx.reading(delta_home_gx);
  imu_avg_home_gy.reading(delta_home_gy);
  imu_avg_home_gz.reading(delta_home_gz);

}

void imuResetMovingAverages() {
  imu_avg_data_ax.reset();
  imu_avg_data_ay.reset();
  imu_avg_data_az.reset();
  imu_avg_data_gx.reset();
  imu_avg_data_gy.reset();
  imu_avg_data_gz.reset();
  imu_avg_home_ax.reset();
  imu_avg_home_ay.reset();
  imu_avg_home_az.reset();
  imu_avg_home_gx.reset();
  imu_avg_home_gy.reset();
  imu_avg_home_gz.reset();
}

void imuUpdateAvgValues() {

  // -- raw data avg
  imu_avg.ax = imu_avg_data_ax.getAvg();
  imu_avg.ay = imu_avg_data_ay.getAvg();
  imu_avg.az = imu_avg_data_az.getAvg();
  imu_avg.gx = imu_avg_data_gx.getAvg();
  imu_avg.gy = imu_avg_data_gy.getAvg();
  imu_avg.gz = imu_avg_data_gz.getAvg();
  imu_avg.last_data = millis();

  // -- home delta avg
  imu_delta_home_avg.ax = imu_avg_home_ax.getAvg();
  imu_delta_home_avg.ay = imu_avg_home_ay.getAvg();
  imu_delta_home_avg.az = imu_avg_home_az.getAvg();
  imu_delta_home_avg.gx = imu_avg_home_gx.getAvg();
  imu_delta_home_avg.gy = imu_avg_home_gy.getAvg();
  imu_delta_home_avg.gz = imu_avg_home_gz.getAvg();
  imu_delta_home_avg.last_data = millis();

}

// ------------------------------------


void Task_IMU_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_IMU, (TickType_t)10) == pdTRUE) {
      
      updateIMU();

      if(DEBUG_IMU_RTOS == true && millis()-last_imu_rtos_print >= 1000) {
        Serial << "imu stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
        last_imu_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_IMU);
    }
    
    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_IMU != 0) vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_IMU );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setIMUTaskPriority(uint8_t p) {

  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_IMU);
  vTaskPrioritySet(Task_IMU, p);
  if(DEBUG_IMU_RTOS) Serial << "changed IMU task priority - new: " << p << " prev: " << prev_priority << endl;

}

