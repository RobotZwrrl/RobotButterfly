uint16_t getSensor_Battery(struct Sensor *s) {
  return analogRead(BATT_SIG_PIN);
}


void updateSensor_Battery(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger
  // TODO
  /*
  if(abs(s->val - s->val_prev) >= LIGHT_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99 && millis()-s->last_val >= LIGHT_WARMUP) { // 5 seconds to warm up
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
  */
  // --

  // -- ambient check
  // TODO
  /*
  if(s->ambient_data[5] != -99) { // see that the data has been populated

    // compare the data from 5 mins ago to now
    // and do this comparison every 1 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= LIGHT_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60) ) { // 1 min wait
      sensorLightAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  */
  // --

}


void initSensor_Battery(struct Sensor *s) {

  s->id = SENSOR_ID_BATTERY;
  s->name = "Battery";
  s->print = false;
  s->print_frequency = 1000;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Battery;
  s->updateSensor = updateSensor_Battery;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_battery.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_battery.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

