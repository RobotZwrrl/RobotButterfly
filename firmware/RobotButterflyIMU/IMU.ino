/*
- get the raw data
- calculate the raw data delta
- add the delta to a moving average filter
- save the average delta every 0.1 second
- compare the deltas in the past 5 seconds (10 deltas)
- printout the past 10 deltas as a csv for analysis
- classify the movement based on thresholds
*/


void updateIMU() {

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
        Serial << "waiting for the robot to settle " << millis()-settle_start << " / " << IMU_HOME_WAIT_TIME << endl;
        last_imu_calib_print = millis();
      }
    }

    // wait for it to settle on startup
    // (eg, after pressing the on/off switch)
    if(millis()-settle_start >= IMU_HOME_SETTLE_TIME) {
      
      if(DEBUG_IMU) Serial << "IMU_SETTLE done, sending to IMU_CALIBRATE_HOME" << endl;

      // clear it out before sending to calibration
      imu_avg_ax.reset();
      imu_avg_ay.reset();
      imu_avg_az.reset();
      imu_avg_gx.reset();
      imu_avg_gy.reset();
      imu_avg_gz.reset();
      
      IMU_STATE = IMU_CALIBRATE_HOME;
      last_home_calibration = millis();
      return;
    }

    return;
  }

  // calibrate the home position
  if(IMU_STATE == IMU_CALIBRATE_HOME) {

    imu_avg_ax.reading(imu.ax);
    imu_avg_ay.reading(imu.ay);
    imu_avg_az.reading(imu.az);
    imu_avg_gx.reading(imu.gx);
    imu_avg_gy.reading(imu.gy);
    imu_avg_gz.reading(imu.gz);

    if(DEBUG_IMU) {
      if(millis()-last_imu_calib_print >= 100) {
        Serial << "calibrating... " << millis()-last_home_calibration << " / " << IMU_HOME_CALIBRATION_TIME << endl;
        last_imu_calib_print = millis();
      }
    }

    // last_home_calibration is the start time of the calibration
    if(millis()-last_home_calibration >= IMU_HOME_CALIBRATION_TIME) {

      // set the home values
      imu_home.ax = imu_avg_ax.getAvg();
      imu_home.ay = imu_avg_ay.getAvg();
      imu_home.az = imu_avg_az.getAvg();
      imu_home.gx = imu_avg_gx.getAvg();
      imu_home.gy = imu_avg_gy.getAvg();
      imu_home.gz = imu_avg_gz.getAvg();
      imu_home.last_data = millis();

      // reset moving average filter as it is repurposed for deltas
      imu_avg_ax.reset();
      imu_avg_ay.reset();
      imu_avg_az.reset();
      imu_avg_gx.reset();
      imu_avg_gy.reset();
      imu_avg_gz.reset();
      last_home_calibration = 0;

      if(DEBUG_IMU) Serial << "calibration done, sending to IMU_ACTIVE" << endl;

      IMU_STATE = IMU_ACTIVE;
      return;
    }

    return;
  }


  // for the delta
  int delta_ax;
  int delta_ay;
  int delta_az;
  int delta_gx;
  int delta_gy;
  int delta_gz;

  // calculate the delta
  if(DELTA_HOME) {
    delta_ax = imu.ax-imu_home.ax;
    delta_ay = imu.ay-imu_home.ay;
    delta_az = imu.az-imu_home.az;
    delta_gx = imu.gx-imu_home.gx;
    delta_gy = imu.gy-imu_home.gy;
    delta_gz = imu.gz-imu_home.gz;
  } else {
    delta_ax = imu.ax-imu_prev.ax;
    delta_ay = imu.ay-imu_prev.ay;
    delta_az = imu.az-imu_prev.az;
    delta_gx = imu.gx-imu_prev.gx;
    delta_gy = imu.gy-imu_prev.gy;
    delta_gz = imu.gz-imu_prev.gz;
  }

  // put the delta into the moving average filter
  imu_avg_ax.reading(delta_ax);
  imu_avg_ay.reading(delta_ay);
  imu_avg_az.reading(delta_az);
  imu_avg_gx.reading(delta_gx);
  imu_avg_gy.reading(delta_gy);
  imu_avg_gz.reading(delta_gz);

  // get the average after 0.5 seconds,
  // hardware timer interrupt sets this flag
  if(new_avg_sample) {

    // get the values
    imu_delta_avg.ax = imu_avg_ax.getAvg();
    imu_delta_avg.ay = imu_avg_ay.getAvg();
    imu_delta_avg.az = imu_avg_az.getAvg();
    imu_delta_avg.gx = imu_avg_gx.getAvg();
    imu_delta_avg.gy = imu_avg_gy.getAvg();
    imu_delta_avg.gz = imu_avg_gz.getAvg();
    imu_delta_avg.last_data = millis();

    // get ready for the next sample
    imu_avg_ax.reset();
    imu_avg_ay.reset();
    imu_avg_az.reset();
    imu_avg_gx.reset();
    imu_avg_gy.reset();
    imu_avg_gz.reset();

    new_avg_sample = false;
  }

  // prints
  if(IMU_DATA_PRINT_RAW) {
    Serial.print("a/g:\t");
    Serial.print(imu.ax); Serial.print("\t");
    Serial.print(imu.ay); Serial.print("\t");
    Serial.print(imu.az); Serial.print("\t");
    Serial.print(imu.gx); Serial.print("\t");
    Serial.print(imu.gy); Serial.print("\t");
    Serial.println(imu.gz);
  }

  if(IMU_DATA_PRINT_DELTA_AVG) {
    Serial.print("a/g:\t");
    Serial.print(imu_delta_avg.ax); Serial.print("\t");
    Serial.print(imu_delta_avg.ay); Serial.print("\t");
    Serial.print(imu_delta_avg.az); Serial.print("\t");
    Serial.print(imu_delta_avg.gx); Serial.print("\t");
    Serial.print(imu_delta_avg.gy); Serial.print("\t");
    Serial.println(imu_delta_avg.gz);
  }

  if(IMU_DATA_PRINT) {
    Serial.print("a/g:\t");
    Serial.print(imu.ax * mpu.get_acce_resolution(), 3); Serial.print("\t");
    Serial.print(imu.ay * mpu.get_acce_resolution(), 3); Serial.print("\t");
    Serial.print(imu.az * mpu.get_acce_resolution(), 3); Serial.print("\t");
    Serial.print(imu.gx * mpu.get_gyro_resolution(), 3); Serial.print("\t");
    Serial.print(imu.gy * mpu.get_gyro_resolution(), 3); Serial.print("\t");
    Serial.println(imu.gz * mpu.get_gyro_resolution(), 3);
  }

}


void initIMU() {

  Serial << "Initializing MPU...";
  mpu.initialize();

  Serial << "Testing MPU6050 connection...";
  if(mpu.testConnection() ==  false){
    Serial << "MPU6050 connection failed";
    //while(true);
  } else {
    Serial << "MPU6050 connection successful" << endl;;
  }

  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

  // TODO: uncomment this
  //IMU_STATE = IMU_SETTLE;
  //settle_start = millis();

  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer2_cfg = timerBegin(2, 16000, true);
  timerAttachInterrupt(timer2_cfg, &Timer2_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer2_cfg, 2500, true);
  timerAlarmEnable(timer2_cfg);

  imu_home.ax = 0;
  imu_home.ay = 0;
  imu_home.az = 0;
  imu_home.gx = 0;
  imu_home.gy = 0;
  imu_home.gz = 0;
  imu_home.last_data = 0;

  imu_avg_ax.begin();
  imu_avg_ay.begin();
  imu_avg_az.begin();
  imu_avg_gx.begin();
  imu_avg_gy.begin();
  imu_avg_gz.begin();

}


