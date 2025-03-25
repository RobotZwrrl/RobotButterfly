
uint16_t getSensor_Temperature(struct Sensor *s) {
  int val = dht11.readTemperature();
  if(val == DHT11::ERROR_CHECKSUM || val == DHT11::ERROR_TIMEOUT) return 99;
  return val;
}


void updateSensor_Temperature(struct Sensor *s) {

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
  if(abs(s->val - s->val_prev) >= TEMPERATURE_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99 && millis()-s->last_val >= TEMPERATURE_WARMUP) { // 10 seconds to warm up
    if(s->val > s->val_prev) { // see if going from cold to warm or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = false;
        sensorTemperatureChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = true;
        sensorTemperatureChangeCallback(s, s->trigger_dir);
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
    Serial << millis() << " Temperature \t RAW: " << s->raw << " (" << raw_iteration << "/" << raw_reload << ")";
    Serial << " \t VAL: " << s->val << " (" << val_iteration << "/" << val_reload << ")";
    Serial << " \t AMBIENT: " << s->ambient << " (" << ambient_iteration << "/" << ambient_reload << ") ";
    
    if(s->ambient_data[5] != -99) {
      Serial << "math: " << s->ambient_data[5] - s->ambient_data[0] << endl;
    } else {
      Serial << endl;
    }
    
    s->last_print = millis();
  }
  // --

}


void initSensor_Temperature(struct Sensor *s) {

  s->id = SENSOR_ID_TEMPERATURE;
  s->print = true;
  
  s->reload_raw = 1*5;          // every 0.2 seconds
  s->reload_val = 10*5;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Temperature;
  s->updateSensor = updateSensor_Temperature;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_temperature.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_temperature.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

