/*
 * Servo Motors
 */ 

void moveLeftWing() {
  // TODO
  // create an animation frame here and send it to the queue
}

void moveRightWing() {
  // TODO
}

void moveBothWings() {
  // TODO
}

void animationDone() {
  // TODO
}

void frameDone() {
  // TODO
}


void initServos() {

  if(!initialised_servos) {
    
    wing_left.motor = s1;
    wing_left.motor.attach(SERVO_LEFT_PIN, SERVO_LEFT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    wing_left.motor.setEasingType(EASE_QUADRATIC_IN_OUT);
    wing_left.active = true;
    wing_left.name = "LEFT";
    
    wing_right.motor = s2;
    wing_right.motor.attach(SERVO_RIGHT_PIN, SERVO_RIGHT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
    wing_right.motor.setEasingType(EASE_QUADRATIC_IN_OUT);
    wing_right.active = true;
    wing_right.name = "RIGHT";

    if(SERVO_LEFT_HOME == -99) {
      Serial << "!! Servo LEFT needs calibration" << endl;
      wing_left.calibrated = false;
    }

    wing_left.home_pos = SERVO_LEFT_HOME;
    wing_left.up_pos = SERVO_LEFT_UP;
    wing_left.down_pos = SERVO_LEFT_DOWN;
    wing_left.current_pos = SERVO_LEFT_UP;
    wing_left.motor.startEaseTo( SERVO_LEFT_UP, 10 );

    if(SERVO_RIGHT_HOME == -99) {
      Serial << "!! Servo RIGHT needs calibration" << endl;
      wing_right.calibrated = false;
    }

    wing_right.home_pos = SERVO_RIGHT_HOME;
    wing_right.up_pos = SERVO_RIGHT_UP;
    wing_right.down_pos = SERVO_RIGHT_DOWN;
    wing_right.motor.startEaseTo( SERVO_RIGHT_UP, 10 );

    initialised_servos = true;
  }

  if(Queue_SM1 == NULL && Queue_SM2 == NULL) {
    // create the queue with 1 slots of 4 bytes
    // more slots = more numbers
    Queue_SM1 = xQueueCreate(1, sizeof(struct Animation));
    Queue_SM2 = xQueueCreate(1, sizeof(uint8_t));
  }
  
  //disableCore0WDT();

  // core 0 has task watchdog enabled to protect wifi service etc
  // core 1 does not have watchdog enabled
  // can do this if wdt gives trouble: disableCore0WDT();
  xTaskCreatePinnedToCore(
                    Task_SM_code,  // task function
                    "Task_SM",     // name of task
                    STACK_SM,      // stack size of task
                    NULL,          // parameter of the task
                    1,             // priority of the task
                    &Task_SM,      // task handle to keep track of created task
                    1);            // pin task to core 1

}


void Task_SM_code(void * pvParameters) {

  if(DEBUG_SM) Serial << "Task_SM_code" << endl;
  
  while(1) {

    //if(DEBUG_SM) Serial << "Task_SM_code" << endl;

    if(millis() < STARTUP_WAIT_SM) continue; // let servos warm up

    // -------------------------- part 1

    // check the queues
    uint8_t ready = 0;
    struct Animation *Anim;
    uint8_t ANIM_STATE;
    
    // check the queues
    if(uxQueueMessagesWaiting(Queue_SM1) > 0) ready++;
    if(uxQueueMessagesWaiting(Queue_SM2) > 0) ready++;
    
    if(ready >= 2) {
      // read the queues
      xQueueReceive(Queue_SM1, &Anim, 0);
      xQueueReceive(Queue_SM2, &ANIM_STATE, 0);
      valuesSM_count++;

      if(DEBUG_SM) Serial << millis() << " [" << xPortGetCoreID() << "] ";
      if(DEBUG_SM) Serial << " Received from queue SM (" << valuesSM_count << ") ANIM_STATE: " << ANIM_STATE << endl;
      // if(DEBUG_SM) Serial << "Frame 0 dwell time: " << Anim->dwell[0] << endl;
      // if(DEBUG_SM) Serial << "Actual 0 dwell time: " << GentleFlap.dwell[0] << endl;
      // if(DEBUG_SM) Serial << "Frame 0 servo L: " << Anim->servo_L[0] << endl;
      // if(DEBUG_SM) Serial << "Actual 0 servo L: " << GentleFlap.servo_L[0] << endl;

      // flush the queues
      xQueueReset(Queue_SM1);
      xQueueReset(Queue_SM2);
    }

    // -------------------------- part 2

    // state check: 1 = go, 0 = stop
    if(ANIM_STATE == 0) {
      continue;
    }

    // dwell wait at the end of the frame
    if(millis()-Anim->start >= Anim->dwell[Anim->index]) {
      // done! move to next frame
      if(DEBUG_SM) Serial << millis() << " Animation done dwell" << endl;
      Anim->active = false;
      Anim->index++;
      if(Anim->index >= Anim->frames) {
        Anim->index = 0;
      }
    }

    // servo update
    if(Anim->active == true) {
      // source: https://github.com/ArminJo/ServoEasing/blob/bf7dedab267e11fc44b130e04542a2b7f4118343/examples/ThreeServos/ThreeServos.ino#L154
      do {
        vTaskDelay( REFRESH_INTERVAL_MILLIS / portTICK_PERIOD_MS );
        //delay(20); // optional 20ms delay
      } while (!updateAllServos());
    }

    // set the target positions
    if(Anim->active == false) {

      if(DEBUG_SM) Serial << millis() << " Animation frame " << Anim->index << " start" << endl;

      if(Anim->servo_L[Anim->index] != 9999) {
        if(DEBUG_SM) Serial << "Servo L: " << Anim->servo_L[Anim->index] << endl;
        wing_left.motor.setEaseTo(Anim->servo_L[Anim->index], Anim->velocity[Anim->index]);
      }

      if(Anim->servo_R[Anim->index] != 9999) {
        if(DEBUG_SM) Serial << "Servo R: " << Anim->servo_R[Anim->index] << endl;
        wing_right.motor.setEaseTo(Anim->servo_R[Anim->index], Anim->velocity[Anim->index]);
      }

      synchronizeAllServosAndStartInterrupt(false);
      Anim->active = true;
      Anim->start = millis();
    }
    
    vTaskDelay(1);
  }

  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}




/*
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
*/

