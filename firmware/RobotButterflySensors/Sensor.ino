void updateSensors() {
  
  // acquire the raw sensor reading
  // add raw to the moving average
  if(sensor_light.update_raw) {
    sensor_light.raw_prev = sensor_light.raw;
    sensor_light.raw = analogRead(LDR_PIN);
    sensor_light_raw.reading(sensor_light.raw);
    sensor_light.last_raw = millis();
    sensor_light.update_raw = false;

    // --
    if(sensor_light.last_val <= 5000) return; // wait to warm up

    if(abs(sensor_light.raw_prev-sensor_light.raw) >= LIGHT_CHANGE_THRESH) {
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
  // filter. that gets added every 1 second.
  if(sensor_light.update_val) {
    sensor_light.val_prev = sensor_light.val;
    sensor_light.val = sensor_light_raw.getAvg();
    sensor_light_raw.reset();

    sensor_light_ambient.reading(sensor_light.val);

    sensor_light.last_val = millis();
    sensor_light.update_val = false;

    // --
    if(sensor_light.print) Serial << millis() << " Light sensor \t RAW: " << sensor_light.raw << " \t VAL: " << sensor_light.val << " \t AMBIENT: " << sensor_light.ambient << endl;
    // --
  }

  // store the ambient value into the array
  if(sensor_light.update_ambient) {
    sensor_light.ambient = sensor_light_ambient.getAvg();
    sensor_light_ambient.reset();

    for(uint8_t i=NUM_AMBIENT_VALS-1; i>0; i--) {
      sensor_light.ambient_vals[i] = sensor_light.ambient_vals[i-1];
    }
    sensor_light.ambient_vals[0] = sensor_light.ambient;

    sensor_light.last_ambient = millis();
    sensor_light.update_ambient = false;
  }

}


void initSensors() {

  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 250, true);
  timerAlarmEnable(timer_10Hz_config);

  sensor_light.id = SENSOR_ID_LIGHT;
  sensor_light.print = true;
  
  sensor_light.reload_raw = 1;       // every 0.1 seconds
  sensor_light.reload_val = 20;      // every 1 seconds
  sensor_light.reload_ambient = 100;  // every 5 seconds

  for(uint8_t i=0; i<NUM_AMBIENT_VALS; i++) {
    sensor_light.ambient_vals[i] = -99;
  }

  sensor_light_raw.begin();
  sensor_light_ambient.begin();

  all_sensors[SENSOR_ID_LIGHT] = &sensor_light;

}

