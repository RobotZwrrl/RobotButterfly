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

}


void updateSensor_Light(struct Sensor *s) {

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

  // -- trigger
  if(abs(s->val - s->val_prev) >= LIGHT_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99) {
    if(s->val > s->val_prev) { // see if going from dark to light or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = false;
        sensorLightChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = true;
        sensorLightChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- ambient check
  if(s->ambient_data[5] != -99) { // see that the data has been populated

    // compare the data from 5 mins ago to now
    // and do this comparison every 1 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= LIGHT_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60) ) { // 1 min wait
      sensorLightAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

  // -- print
  if(s->print == true && millis()-s->last_print >= 1000) {
    Serial << millis() << " Light sensor \t RAW: " << s->raw << " (" << raw_iteration << "/" << raw_reload << ")";
    Serial << " \t VAL: " << s->val << " (" << val_iteration << "/" << val_reload << ")";
    Serial << " \t AMBIENT: " << s->ambient << " (" << ambient_iteration << "/" << ambient_reload << ")" << endl;
    
    if(s->ambient_data[5] != -99) Serial << "ambient math: " << s->ambient_data[5] - s->ambient_data[0] << endl;
    
    s->last_print = millis();
  }
  // --

}


void initSensor_Temperature(struct Sensor *s) {

  s->id = SENSOR_ID_LIGHT;
  s->print = true;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Light;
  s->updateSensor = updateSensor_Light;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_light.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_light.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}


void initSensors() {

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    all_sensors[i] = NULL;
  }

  all_sensors[SENSOR_ID_LIGHT] = &sensor_light;
  initSensor_Temperature(all_sensors[SENSOR_ID_LIGHT]);

  
  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

}

