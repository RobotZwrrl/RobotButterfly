uint16_t getSensor_Light(struct Sensor *s) {
  return analogRead(LDR_PIN);
}

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
    s->raw = s->getRawData(s);
    s->val_avg.reading(s->raw);
    s->last_raw = millis();
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
    noInterrupts();
      s->update_ambient = false;
    interrupts();
    s->ambient_prev = s->ambient;
    s->ambient = s->ambient_avg.getAvg();
    s->ambient_avg.reset();
    s->last_ambient = millis();
  }

}


void updateSensors() {

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;
    processSensors(s);
  }

  updateLightSensor( all_sensors[SENSOR_ID_LIGHT] );

}


void updateLightSensor(struct Sensor *s) {

  if(s == NULL) return;

  uint8_t raw_iteration;
  uint8_t raw_reload;
  uint8_t val_iteration;
  uint8_t val_reload;
  uint16_t ambient_iteration;
  uint16_t ambient_reload;

  // make it atomic by copying to local variables
  noInterrupts();
    raw_iteration = s->iteration_raw;
    raw_reload = s->reload_raw;
    val_iteration = s->iteration_val;
    val_reload = s->reload_val;
    ambient_iteration = s->iteration_ambient;
    ambient_reload = s->reload_ambient;
  interrupts();

  // --
  if(abs(s->val - s->val_prev) >= LIGHT_CHANGE_THRESH // val_avg.getAvg()
    && s->last_val != -99
    && millis()-s->last_sensor_trigger >= 1001) {
    if(s->val > s->val_prev) {
      Serial << "light on!" << endl;
    } else {
      Serial << "light off!" << endl;
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // --
  if(s->print == true && millis()-s->last_print >= 1000) {
    Serial << millis() << " Light sensor \t RAW: " << s->raw << " (" << raw_iteration << "/" << raw_reload << ")";
    Serial << " \t VAL: " << s->val << " (" << val_iteration << "/" << val_reload << ")";
    Serial << " \t AMBIENT: " << s->ambient << " (" << ambient_iteration << "/" << ambient_reload << ")" << endl;
    s->last_print = millis();
  }
  // --

  // if(abs(s->ambient_prev-s->ambient) >= LIGHT_AMBIENT_THRESH
  //   && s->last_ambient != -99) {
  //   Serial << "ambient light change detected!" << endl;
  // }

  /*
  // acquire the raw sensor reading
  // add raw to the moving average
  if(sensor_light.update_raw) {
    sensor_light.raw_prev = sensor_light.raw;
    sensor_light.raw = analogRead(LDR_PIN);
    sensor_light_raw.reading(sensor_light.raw);
    sensor_light.last_raw = millis();
    sensor_light.update_raw = false;

    // --
    if(abs(sensor_light.raw_prev-sensor_light.raw) >= LIGHT_CHANGE_THRESH
      && sensor_light.last_val != -99) {
      if(sensor_light.raw > sensor_light.raw_prev) {
        Serial << "light on!" << endl;
      } else {
        Serial << "light off!" << endl;
      }
    }
    // --
  }

  // store the moving average filter with the raw 
  // sensor data to `val`. reset the filter.
  // then, add `val` to the ambient moving average
  // filter every 1 second.
  if(sensor_light.update_val) {
    sensor_light.val_prev = sensor_light.val;
    sensor_light.val = sensor_light_raw.getAvg();
    sensor_light_raw.reset();

    sensor_light_ambient.reading(sensor_light.val);

    sensor_light.last_val = millis();
    sensor_light.update_val = false;

    // --
    if(sensor_light.print) Serial << millis() << " Light sensor \t RAW: " << sensor_light.raw << " (" << sensor_light.iteration_raw << "/" << sensor_light.reload_raw << ")";
    if(sensor_light.print) Serial << " \t VAL: " << sensor_light.val << " (" << sensor_light.iteration_val << "/" << sensor_light.reload_val << ")";
    if(sensor_light.print) Serial << " \t AMBIENT: " << sensor_light.ambient << " (" << sensor_light.iteration_ambient << "/" << sensor_light.reload_ambient << ")" << endl;
    // --
  }

  // store the ambient value every 60 seconds
  if(sensor_light.update_ambient) {
    sensor_light.ambient_prev = sensor_light.ambient;
    sensor_light.ambient = sensor_light_ambient.getAvg();
    sensor_light_ambient.reset();

    sensor_light.last_ambient = millis();
    sensor_light.update_ambient = false;

    if(abs(sensor_light.ambient_prev-sensor_light.ambient) >= LIGHT_AMBIENT_THRESH
       && sensor_light.last_ambient != -99) {
      Serial << "ambient light change detected!" << endl;
    }

  }
  */

}



void initSensors() {

  sensor_light.id = SENSOR_ID_LIGHT;
  sensor_light.print = true;
  
  sensor_light.reload_raw = 1;          // every 0.1 seconds
  sensor_light.reload_val = 10;         // every 1 seconds
  sensor_light.reload_ambient = 600;    // every 60 seconds

  sensor_light.last_val = -99;
  sensor_light.last_ambient = -99;

  sensor_light.val_avg.begin();
  sensor_light.ambient_avg.begin();

  //sensor_light.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);

  sensor_light.getRawData = getSensor_Light;

  sensor_light_raw.begin();
  sensor_light_ambient.begin();

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    all_sensors[i] = NULL;
  }
  all_sensors[SENSOR_ID_LIGHT] = &sensor_light;

  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

}

