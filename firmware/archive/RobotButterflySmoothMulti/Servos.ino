/*
 * Servo Motors
 */ 

void moveLeftWing(uint16_t pos) {

  struct Animation temp;

  // take mutex prior to critical section
  if(xSemaphoreTake(Mutex_SM, (TickType_t)10) == pdTRUE) {
    // critical section
    temp = PresentAnimation;
    // give mutex after critical section
    xSemaphoreGive(Mutex_SM);
  } else {
    if(WARN_SM) Serial << "!! error A1" << endl;
    return;
  }

  struct Animation a;

  a.id = 99;
  a.frames = 1;
  a.start = 0;
  a.active = false;
  a.index = 0;
  a.loop = false;
  a.reverse = false;
  a.done = false;

  a.servo_L[0] = pos; 
  a.servo_R[0] = temp.servo_R[temp.index];
  a.velocity[0] = 50;
  a.dwell[0] = SERVO_MIN_DWELL;

  sendAnimation(&a, ANIM_PLAY);

}

void moveRightWing(uint16_t pos) {
  
  struct Animation temp;

  // take mutex prior to critical section
  if(xSemaphoreTake(Mutex_SM, (TickType_t)10) == pdTRUE) {
    // critical section
    temp = PresentAnimation;
    // give mutex after critical section
    xSemaphoreGive(Mutex_SM);
  } else {
    if(WARN_SM) Serial << "!! error A2" << endl;
    return;
  }

  struct Animation a;

  a.id = 99;
  a.frames = 1;
  a.start = 0;
  a.active = false;
  a.index = 0;
  a.loop = false;
  a.reverse = false;
  a.done = false;

  a.servo_L[0] = temp.servo_L[temp.index]; 
  a.servo_R[0] = pos;
  a.velocity[0] = 50;
  a.dwell[0] = SERVO_MIN_DWELL;

  sendAnimation(&a, ANIM_PLAY);

}

void moveBothWings(uint16_t pos_left, uint16_t pos_right) {
  
  struct Animation a;

  a.id = 99;
  a.frames = 1;
  a.start = 0;
  a.active = false;
  a.index = 0;
  a.loop = false;
  a.reverse = false;
  a.done = false;

  a.servo_L[0] = pos_left; 
  a.servo_R[0] = pos_right;
  a.velocity[0] = 50;
  a.dwell[0] = SERVO_MIN_DWELL;

  sendAnimation(&a, ANIM_PLAY);

}



void initServos() {

  if(!initialised_servos) {
    
    Mutex_SM = xSemaphoreCreateMutex();
    semaphore_anim_frame = xSemaphoreCreateBinary();
    semaphore_anim_loop = xSemaphoreCreateBinary();
    semaphore_anim_done = xSemaphoreCreateBinary();
    semaphore_anim_new = xSemaphoreCreateBinary();

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

  uint8_t ready = 0;
  static struct Animation Anim;
  uint8_t LOCAL_ANIM_STATE;

  if(DEBUG_SM) Serial << "Task_SM_code" << endl;
  
  while(1) {

    //if(DEBUG_SM) Serial << "Task_SM_code" << endl;

    if(millis() < STARTUP_WAIT_SM) continue; // let servos warm up

    // -------------------------- part 1

    // event driven with this 'new anim' mutex, 
    // rather than polling for the critical section mutex
    // - and taking it (crit section mutex) in & out continuously
    if(xSemaphoreTake(semaphore_anim_new, (TickType_t)10) == pdTRUE) {
      
      // take mutex prior to critical section
      if(xSemaphoreTake(Mutex_SM, (TickType_t)10) == pdTRUE) {
        
        //if(DEBUG_SM) Serial << "Mutex_SM taken" << endl;

        // -- critical selection
        Anim = PresentAnimation;
        LOCAL_ANIM_STATE = ANIM_STATE;
        // --

        if(DEBUG_SM) Serial << "ID: " << Anim.id << endl;
        if(DEBUG_SM) Serial << "Actual ID: " << GentleFlap.id << endl;
        if(DEBUG_SM) Serial << "Actual ID B: " << PresentAnimation.id << endl;
        if(DEBUG_SM) Serial << "Frames: " << Anim.frames << endl;
        if(DEBUG_SM) Serial << "Actual frames: " << GentleFlap.frames << endl;
        if(DEBUG_SM) Serial << "Actual frames B: " << PresentAnimation.frames << endl;
        // if(DEBUG_SM) Serial << "Frame 0 dwell time: " << Anim.dwell[0] << endl;
        // if(DEBUG_SM) Serial << "Actual 0 dwell time: " << GentleFlap.dwell[0] << endl;
        // if(DEBUG_SM) Serial << "Frame 1 dwell time: " << Anim.dwell[1] << endl;
        // if(DEBUG_SM) Serial << "Actual 1 dwell time: " << GentleFlap.dwell[1] << endl;
        // if(DEBUG_SM) Serial << "Frame 0 servo L: " << Anim.servo_L[0] << endl;
        // if(DEBUG_SM) Serial << "Actual 0 servo L: " << GentleFlap.servo_L[0] << endl;

        // give mutex after critical section
        xSemaphoreGive(Mutex_SM);
        
      } else {
        continue; // skip this iteration
      }

    } else {
      
      // if there's no new animation, then post our update to the global variable
      
      // TODO: what to do if it doesn't get the mutex?
      // take mutex prior to critical section
      if(xSemaphoreTake(Mutex_SM, (TickType_t)10) == pdTRUE) {
        // critical section
        PresentAnimation = Anim;
        // give mutex after critical section
        xSemaphoreGive(Mutex_SM);
      }

    }


    // -------------------------- part 2

    // state check
    if(LOCAL_ANIM_STATE == ANIM_STOP || Anim.done == true) {
      continue;
    }

    if(Anim.done == true) {
      continue;
    }

    // dwell wait at the end of the frame
    if(millis()-Anim.start >= Anim.dwell[Anim.index] && Anim.start > 0) {
      // done! move to next frame
      //if(DEBUG_SM) Serial << millis() << " Animation done dwell" << endl;
      Anim.active = false;
      Anim.done = false;
      Anim.index++;
      xSemaphoreGive(semaphore_anim_frame); // send the semaphore that it's the next frame

      if(Anim.index >= Anim.frames) { // check for last frame
        Anim.index = 0;
        if(Anim.loop == false) {
          Anim.active = false;
          Anim.done = true;          
          xSemaphoreGive(semaphore_anim_done); // send the semaphore that the animation is done
        } else {
          Anim.active = false;
          Anim.done = false; 
          xSemaphoreGive(semaphore_anim_loop); // send the semaphore that the animation is looped
        }
      }
    }

    // -------------------------- part 3

    // servo update
    if(Anim.active == true) {
      // source: https://github.com/ArminJo/ServoEasing/blob/bf7dedab267e11fc44b130e04542a2b7f4118343/examples/ThreeServos/ThreeServos.ino#L154
      do {
        vTaskDelay( REFRESH_INTERVAL_MILLIS / portTICK_PERIOD_MS );
        //delay(20); // optional 20ms delay
      } while (!updateAllServos());
    }

    // set the target positions
    if(Anim.active == false && Anim.done == false) {

      if(DEBUG_SM) Serial << millis() << " Animation frame " << Anim.index << " start" << endl;

      if(Anim.servo_L[Anim.index] != 9999) {
        if(DEBUG_SM) Serial << "Servo L: " << Anim.servo_L[Anim.index] << endl;
        wing_left.motor.setEaseTo(Anim.servo_L[Anim.index], Anim.velocity[Anim.index]);
      }

      if(Anim.servo_R[Anim.index] != 9999) {
        if(DEBUG_SM) Serial << "Servo R: " << Anim.servo_R[Anim.index] << endl;
        wing_right.motor.setEaseTo(Anim.servo_R[Anim.index], Anim.velocity[Anim.index]);
      }

      synchronizeAllServosAndStartInterrupt(false);
      Anim.active = true;
      Anim.start = millis();

    }

    vTaskDelay(1);
  }

  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


