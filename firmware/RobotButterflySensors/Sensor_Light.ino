uint16_t getSensor_Light(struct Sensor *s) {
  return analogRead(LDR_PIN);
}


void updateSensor_Light(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger
  if(abs(s->val - s->val_prev) >= LIGHT_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99 && s->val_prev != 0) {
    if(s->val > s->val_prev) { // see if going from dark to light or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
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
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Light(struct Sensor *s) {

  s->id = SENSOR_ID_LIGHT;
  s->name = "Light";
  s->print = false;
  s->print_frequency = 1000;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Light;
  s->updateSensor = updateSensor_Light;

  s->onSensorChangeCallback = sensorLightChangeCallback;
  s->onSensorAmbientChangeCallback = sensorLightAmbientChangeCallback;

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

