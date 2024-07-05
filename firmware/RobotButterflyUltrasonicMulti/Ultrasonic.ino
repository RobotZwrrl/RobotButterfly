/*
 * Ultrasonic Sensor (HC-SR04)
 */ 

void initUltrasonic() {

  if(!initialised_sensor) {
    HCSR04.begin(ULTRASONIC_OUT_PIN, ULTRASONIC_IN_PIN);
    initialised_sensor = true;
  }

  if(Queue_US1 == NULL && Queue_US2 == NULL && Queue_US3 == NULL) {
    // create the queue with 1 slots of 4 bytes
    // more slots = more numbers
    Queue_US1 = xQueueCreate(1, sizeof(double));
    Queue_US2 = xQueueCreate(1, sizeof(int));
    Queue_US3 = xQueueCreate(1, sizeof(long));
  }

  // core 0 has task watchdog enabled to protect wifi service etc
  // core 1 does not have watchdog enabled
  // can do this if wdt gives trouble: disableCore0WDT();
  xTaskCreatePinnedToCore(
                    Task_US_code,  // task function
                    "Task_US",     // name of task
                    STACK_US,      // stack size of task
                    NULL,          // parameter of the task
                    1,             // priority of the task
                    &Task_US,      // task handle to keep track of created task
                    0);            // pin task to core 0

}


void Task_US_code(void * pvParameters) {

  if(DEBUG_US) Serial << "Task_US_code" << endl;
  
  while(1) {

    if(DEBUG_US) Serial << "Reading HC-SR04" << endl;

    if(millis() < STARTUP_WAIT_US) continue; // let sensor warm up

    double *distances = HCSR04.measureDistanceCm();
    double dist_cm = distances[0];
    
    if(dist_cm < 0) { // error
      Serial << endl;
      Serial << millis() << " [" << xPortGetCoreID() << "] ";
      Serial << "ERROR US" << endl;
      Serial << "---------" << endl;
      vTaskDelay(1);
      continue;
    }

    int dist = (int)floor(dist_cm);
    if(dist > DIST_MAX) dist = DIST_MAX;
    if(dist < DIST_MIN) dist = DIST_MIN;
    int neo_map = (int)map(dist, DIST_MIN, DIST_MAX, 1, 8);

    if(DEBUG_US) Serial << "dist_cm: " << dist_cm << " dist: " << dist << " neo_map: " << neo_map << endl;

    long last_sample = millis();

    if(DEBUG_US) Serial << millis() << " [" << xPortGetCoreID() << "] ";
    if(DEBUG_US) Serial << "Adding to queue US " << dist_cm << ", " << neo_map << ", " << last_sample << endl;

    // add to the queue
    // time of 0 says don't block if the queue is already full
    xQueueSend(Queue_US1, &dist_cm, (TickType_t)0);
    xQueueSend(Queue_US2, &neo_map, (TickType_t)0);
    xQueueSend(Queue_US3, &last_sample, (TickType_t)0);

    if(DEBUG_US) Serial << "task delay now" << endl;
    vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil( &xLastWakeTime, SAMPLE_FREQ_US );
  }

  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void updateUltrasonic() {

  // if wanting to only update based on the interrupt timer, uncomment this block
  // accessing a variable that's updated in interrupts
  // noInterrupts(); // same as portDISABLE_INTERRUPTS();
  // if(!updateUS) return; // not needed, this updates continuously
  // interrupts(); // same as portENABLE_INTERRUPTS();

  if(millis()-last_print_us >= 100) {
    Serial << millis();
    Serial << " dist_cm_raw = " << raw_us.dist_cm_raw;
    Serial << " dist_8_raw = " << raw_us.dist_8_raw;
    Serial << endl;
    last_print_us = millis();
  }

  uint8_t ready = 0;
  double a;
  int b;
  long c;

  // check the queues
  if(uxQueueMessagesWaiting(Queue_US1) > 0) ready++;
  if(uxQueueMessagesWaiting(Queue_US2) > 0) ready++;
  if(uxQueueMessagesWaiting(Queue_US3) > 0) ready++;
  
  if(ready < 3) {
    //Serial << "Queue messages not ready" << endl;
    return;
  }

  // read the queues
  xQueueReceive(Queue_US1, &a, 0);
  xQueueReceive(Queue_US2, &b, 0);
  xQueueReceive(Queue_US3, &c, 0);

  raw_us.dist_cm_raw = a;
  raw_us.dist_8_raw = b;
  raw_us.record_time = c;
  valuesUS_count++;

  if(DEBUG_US) Serial << millis() << " Received from queue US (" << valuesUS_count << "): " << raw_us.dist_cm_raw << ", " << raw_us.dist_8_raw << ", " << raw_us.record_time << endl;
  
  // flush the queues
  xQueueReset(Queue_US1);
  xQueueReset(Queue_US2);
  xQueueReset(Queue_US3);

  // reset flag
  // accessing a variable that's updated in interrupts
  noInterrupts();
  updateUS = false;
  interrupts();

}


