void processSensors(struct Sensor *s) {

  bool raw_update;
  uint8_t raw_iteration;
  uint8_t raw_reload;
  bool val_update;
  uint8_t val_iteration;
  uint8_t val_reload;
  bool ambient_update;
  uint16_t ambient_iteration;
  uint16_t ambient_reload;

  uint16_t raw_temporary;
  bool skip_raw = false;

  // make it atomic by copying to local variables
  noInterrupts();
    raw_update = s->update_raw;
    raw_iteration = s->iteration_raw;
    raw_reload = s->reload_raw;
    val_update = s->update_val;
    val_iteration = s->iteration_val;
    val_reload = s->reload_val;
    ambient_update = s->update_ambient;
    ambient_iteration = s->iteration_ambient;
    ambient_reload = s->reload_ambient;
  interrupts();

  // acquire the raw sensor reading,
  // add raw to the moving average
  if(raw_update) {
    noInterrupts();
      s->update_raw = false;
    interrupts();

    s->raw_prev = s->raw;
    raw_temporary = s->getRawData(s);

    if(s->id == SENSOR_ID_TEMPERATURE || s->id == SENSOR_ID_HUMIDITY) {
      if(raw_temporary == 99) { // bad val
        skip_raw = true;
      }
    }
    
    if(!skip_raw) {
      s->raw = raw_temporary;
      s->val_avg.reading(s->raw);
      s->last_raw = millis();
    }
  }

  // store the moving average filter with the raw 
  // sensor data to `val`. reset the filter.
  // then, add `val` to the ambient moving average
  // filter every 1 second.
  if(val_update) {
    noInterrupts();
      s->update_val = false;
    interrupts();
    s->val_prev = s->val;
    s->val = s->val_avg.getAvg();
    s->val_avg.reset();
    s->ambient_avg.reading(s->val);
    s->last_val = millis();
  }

  // store the ambient value every 60 seconds
  if(ambient_update) {
    //Serial << "\r\n\r\nhello ambient_update " << s->ambient_avg.getAvg() << "\r\n\r\n" << endl;
    noInterrupts();
      s->update_ambient = false;
    interrupts();
    s->ambient_prev = s->ambient;
    s->ambient = s->ambient_avg.getAvg();
    s->ambient_avg.reset();
    s->last_ambient = millis();

    for(uint8_t i=5; i>0; i--) {
      s->ambient_data[i] = s->ambient_data[i-1];
    }
    s->ambient_data[0] = s->ambient;

  }

}


void updateSensors() {

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;
    processSensors(s);
    s->updateSensor(s);
  }

  // processSensors( all_sensors[SENSOR_ID_LIGHT] );
  // all_sensors[SENSOR_ID_LIGHT]->updateSensor( all_sensors[SENSOR_ID_LIGHT] );

  // processSensors( all_sensors[SENSOR_ID_BATTERY] );
  // all_sensors[SENSOR_ID_BATTERY]->updateSensor( all_sensors[SENSOR_ID_BATTERY] );

  // processSensors( all_sensors[SENSOR_ID_SOUND] );
  // all_sensors[SENSOR_ID_SOUND]->updateSensor( all_sensors[SENSOR_ID_SOUND] );

  //processSensors( all_sensors[SENSOR_ID_TEMPERATURE] );
  //all_sensors[SENSOR_ID_TEMPERATURE]->updateSensor( all_sensors[SENSOR_ID_TEMPERATURE] );

}


void initSensors() {

  dht11.setDelay(0); // their library has a blocking delay, set to 0

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    all_sensors[i] = NULL;
  }

  all_sensors[SENSOR_ID_LIGHT] = &sensor_light;
  initSensor_Light(all_sensors[SENSOR_ID_LIGHT]);

  // all_sensors[SENSOR_ID_BATTERY] = &sensor_battery;
  // initSensor_Battery(all_sensors[SENSOR_ID_BATTERY]);

  // all_sensors[SENSOR_ID_SOUND] = &sensor_sound;
  // initSensor_Sound(all_sensors[SENSOR_ID_SOUND]);

  all_sensors[SENSOR_ID_TEMPERATURE] = &sensor_temperature;
  initSensor_Temperature(all_sensors[SENSOR_ID_TEMPERATURE]);

  
  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

}

