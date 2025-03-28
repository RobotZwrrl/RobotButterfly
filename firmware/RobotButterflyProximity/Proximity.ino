void updateProximity() {

  if(!ultrasonic.initialised) return;

  if(millis() < PROXIMITY_WARMUP_TIME) return; // let sensor warm up

  if(!ultrasonic.update_raw) return; // flag set by isr

  ultrasonic.raw_prev = ultrasonic.raw;

  double *distances = HCSR04.measureDistanceCm();
  int dist = (int)floor(distances[0]);
  if(dist > PROXIMITY_MAX) dist = PROXIMITY_MAX;
  if(dist < PROXIMITY_MIN) dist = PROXIMITY_MIN;
  
  ultrasonic.raw = dist;

  if(ultrasonic.raw <= ultrasonic.threshold && millis()-ultrasonic.last_trigger > 500) {
    proximityTriggerCallback(&ultrasonic);
    ultrasonic.last_trigger = millis();
  }

  if(millis()-ultrasonic.last_print >= 100) {
    if(DEBUG_PROXIMITY) Serial << "Proximity: " << ultrasonic.raw << endl;
    ultrasonic.last_print = millis();
  }

  // reset the flag for the isr
  noInterrupts();
    ultrasonic.update_raw = false;
  interrupts();

}


void initProximity() {

  HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);
  ultrasonic.initialised = true;

  ultrasonic.threshold = PROXIMITY_THRESHOLD;
  ultrasonic.last_trigger = 0;
  ultrasonic.last_print = 0;
  ultrasonic.iteration_raw = 0;
  ultrasonic.update_raw = false;
  ultrasonic.reload_raw = 1*5;        // every 0.5 seconds
  
  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

}

