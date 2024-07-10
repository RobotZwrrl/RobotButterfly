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

  if(Queue_SM1 == NULL && Queue_SM2 == NULL && Queue_SM3 == NULL) {
    // create the queue with 1 slots of 4 bytes
    // more slots = more numbers
    Queue_SM1 = xQueueCreate(1, sizeof(Keyframe));
    Queue_SM2 = xQueueCreate(1, sizeof(uint8_t));
    Queue_SM3 = xQueueCreate(1, sizeof(uint8_t));
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
    struct Keyframe Anim;
    uint8_t anim_steps;
    uint8_t anim_index;

    // check the queues
    if(uxQueueMessagesWaiting(Queue_SM1) > 0) ready++;
    if(uxQueueMessagesWaiting(Queue_SM2) > 0) ready++;
    if(uxQueueMessagesWaiting(Queue_SM3) > 0) ready++;
    
    if(ready >= 3) {
      // read the queues
      xQueueReceive(Queue_SM1, &Anim, 0);
      xQueueReceive(Queue_SM2, &anim_steps, 0);
      xQueueReceive(Queue_SM3, &anim_index, 0);
      valuesSM_count++;

      if(DEBUG_SM) Serial << millis() << " [" << xPortGetCoreID() << "] ";
      if(DEBUG_SM) Serial << millis() << " Received from queue SM (" << valuesSM_count << "): " << anim_steps << ", " << anim_index << endl;
      if(DEBUG_SM) Serial << "Frame 1 dwell time: " << Anim.dwell << endl;
      if(DEBUG_SM) Serial << "Lol 1 dwell time: " << Animation[0].dwell << endl;
      if(DEBUG_SM) Serial << "Lolb 1 dwell time: " << HomeFrames[0].dwell << endl;

      // flush the queues
      xQueueReset(Queue_SM1);
      xQueueReset(Queue_SM2);
      xQueueReset(Queue_SM3);
    }

    // -------------------------- part 2

    if(Anim.moving) {
      if(DEBUG_SM) Serial << "moving" << endl;
    } else {
      if(DEBUG_SM) Serial << "not moving" << endl;
    }


    // dwell wait at the end of the frame
    if(millis()-Anim.start >= Anim.dwell) {
      // done! move to next frame
      if(DEBUG_SM) Serial << "Animation done dwell" << endl;
      Anim.moving = false;
      anim_index++;
      if(anim_index >= anim_steps) {
        anim_index = 0;
      }
    }

    if(Anim.moving == false) {

      if(DEBUG_SM) Serial << "Animation frame " << anim_index << " start" << endl;

      if(Anim.servo_L != 9999) {
        wing_left.current_pos = Anim[anim_index].servo_L;
        //wing_left.motor.startEaseTo(wing_left.current_pos, Anim[flap_index].velocity);
      }

      if(Anim.servo_R != 9999) {
        wing_right.current_pos = Anim.servo_R;
        //wing_right.motor.startEaseTo(wing_right.current_pos, Anim[flap_index].velocity);
      }

      Anim.moving = true;
      Anim.start = millis();

      //TickType_t xLastWakeTime = xTaskGetTickCount();
      //vTaskDelayUntil( &xLastWakeTime, Anim[anim_index].dwell );
      
    }
    
    //if(DEBUG_SM) Serial << "Task_SM delay now" << endl;
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

