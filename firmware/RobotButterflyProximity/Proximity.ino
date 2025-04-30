void updateProximity() {

  // entrance checks
  if(!ultrasonic.initialised) return;
  if(millis() < PROXIMITY_WARMUP_TIME) return; // let sensor warm up
  if(!ultrasonic.update_raw) return; // flag set by isr

  // read the sensor and clamp the values
  double *distances = HCSR04.measureDistanceCm();
  int dist = (int)floor(distances[0]);
  if(dist > PROXIMITY_MAX) dist = PROXIMITY_MAX;
  if(dist < PROXIMITY_MIN) dist = PROXIMITY_MIN;
  
  // update raw
  ultrasonic.raw_prev = ultrasonic.raw;
  ultrasonic.raw = dist;

  // moving average filter update
  ultrasonic.val_prev = ultrasonic.val;
  ultrasonic.val_avg.reading(ultrasonic.raw);
  ultrasonic.val = ultrasonic.val_avg.getAvg();

  // detect close proximity trigger
  if(ultrasonic.raw <= ultrasonic.threshold && millis()-ultrasonic.last_trigger > PROXIMITY_TRIGGER_FREQ) {
    if(onProximityTriggerCallback) onProximityTriggerCallback(&ultrasonic);
    ultrasonic.last_trigger = millis();
  }

  // print
  if(millis()-ultrasonic.last_print >= 100) {
    if(DEBUG_PROXIMITY) Serial << "Proximity raw: " << ultrasonic.raw << " avg: " << ultrasonic.val << endl;
    ultrasonic.last_print = millis();
  }

  // reset the flag for the isr
  noInterrupts();
    ultrasonic.update_raw = false;
  interrupts();

}


void initProximity() {

  onProximityTriggerCallback = proximityTriggerCallback;

  HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);
  ultrasonic.initialised = true;

  ultrasonic.threshold = PROXIMITY_THRESHOLD;
  ultrasonic.last_trigger = 0;
  ultrasonic.last_print = 0;
  ultrasonic.iteration_raw = 0;
  ultrasonic.update_raw = false;
  ultrasonic.reload_raw = 1*5;        // every 0.5 seconds
  ultrasonic.val_avg.begin();

  // set up timer 2 for every 0.1 second
  // params: timer #2, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

}

