void updateIMU() {

  // get the new values
  mpu.getMotion6(&imu.ax, &imu.ay, &imu.az, &imu.gx, &imu.gy, &imu.gz);
  imu.last_data = millis();

  if(IMU_DATA_PRINT_RAW) {
    Serial.print("a/g:\t");
    Serial.print(imu.ax); Serial.print("\t");
    Serial.print(imu.ay); Serial.print("\t");
    Serial.print(imu.az); Serial.print("\t");
    Serial.print(imu.gx); Serial.print("\t");
    Serial.print(imu.gy); Serial.print("\t");
    Serial.println(imu.gz);
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

}


