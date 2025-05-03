uint16_t getSensor_Humidity(struct Sensor *s) {
  
  if(s == NULL) return 999;

  int humid_raw = 0;
  float h = 99.9;

  noInterrupts();
    h = dht.readHumidity();
  interrupts();

  if(isnan(h)) {
    humid_raw = 999;
    if(s->print) Serial << "Failed to read from DHT sensor (humidity)" << endl;
  } else {
    humid_raw = (int)(h*10);
    //if(s->print) Serial << "New humidity reading: " << h << ", " << humid_raw << endl;
  }

  return (uint16_t)humid_raw;
}


void updateSensor_Humidity(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger on raw data
  if(abs(s->raw - s->raw_prev) >= HUMIDITY_CHANGE_THRESH // see if the change is great enough
     && s->last_raw != -99 && s->raw_prev != 0) {
    if(s->raw > s->raw_prev) { // see if going from wet to dry or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- trigger on ambient data
  if(s->ambient_data[5] != -99) { // check that the data has been populated

    // compare the data from 10 mins ago to now
    // and do this comparison every 2 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= HUMIDITY_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60*2) // 2 min wait
      && s->ambient_data[0] != 0 && s->ambient_data[5] != 0) { 
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Humidity(struct Sensor *s) {

  s->id = SENSOR_ID_HUMIDITY;
  s->name = "Humidity";
  s->print = true;
  s->print_frequency = 3000;
  
  s->reload_raw = 1*10*3;          // every 3 seconds
  s->reload_val = 10*6*20;         // every 20 seconds
  s->reload_ambient = 600*2;       // every 120 seconds

  // functions
  s->getRawData = getSensor_Humidity;
  s->updateSensor = updateSensor_Humidity;

  s->onSensorChangeCallback = sensorHumidityChangeCallback;
  s->onSensorAmbientChangeCallback = sensorHumidityAmbientChangeCallback;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_humidity.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_humidity.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

