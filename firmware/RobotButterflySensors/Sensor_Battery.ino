uint16_t getSensor_Battery(struct Sensor *s) {
  return analogRead(BATT_SIG_PIN);
}


void updateSensor_Battery(struct Sensor *s) {

  if(s == NULL) return;

  // logic for battery threshold can go here
  // future update

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

