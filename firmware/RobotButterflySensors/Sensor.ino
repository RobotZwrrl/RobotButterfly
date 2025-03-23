uint16_t getSensor_Light(struct Sensor *s) {
  return analogRead(LDR_PIN);
}

void processSensors(struct Sensor *s) {

  bool raw_update;
  bool val_update;
  uint16_t val_iteration;

  noInterrupts();
    raw_update = s->update_raw;
    val_update = s->update_val;
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

    // --
    if(abs(s->raw_prev-s->raw) >= LIGHT_CHANGE_THRESH
      && s->last_val != -99) {
      if(s->raw > s->raw_prev) {
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
  if(val_update) {
    noInterrupts();
      s->update_val = false;
    interrupts();
    s->val_prev = s->val;
    s->val = s->val_avg.getAvg();
    s->val_avg.reset();

    s->ambient_avg.reading(s->val);

    s->last_val = millis();

    // --
    if(s->print) {
      Serial << millis() << " Light sensor \t RAW: " << s->raw << " (" << s->iteration_raw << "/" << s->reload_raw << ")";
      Serial << " \t VAL: " << s->val << " (" << s->iteration_val << "/" << s->reload_val << ")";
      Serial << " \t AMBIENT: " << s->ambient << " (" << s->iteration_ambient << "/" << s->reload_ambient << ")" << endl;
    }
    // --
  }

  // store the ambient value every 60 seconds
  if(s->update_ambient) {
    noInterrupts();
      s->update_ambient = false;
    interrupts();
    s->ambient_prev = s->ambient;
    s->ambient = s->ambient_avg.getAvg();
    s->ambient_avg.reset();

    s->last_ambient = millis();

    if(abs(s->ambient_prev-s->ambient) >= LIGHT_AMBIENT_THRESH
       && s->last_ambient != -99) {
      Serial << "ambient light change detected!" << endl;
    }

  }

}

void updateSensors() {

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;
    processSensors(s);
  }

}


void updateLightSensor() {

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

