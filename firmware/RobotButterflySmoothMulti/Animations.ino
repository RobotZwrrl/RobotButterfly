
// TODO
void callbackFrameDone() {
  // TODO
  //Serial << "---> Callback: Frame done" << endl;
}

void callbackAnimLoop() {
  // TODO
  //Serial << "---> Callback: Animation looped" << endl;
}

void callbackAnimDone() {
  // TODO
  //Serial << "---> Callback: Animation done" << endl;
}


const uint8_t num_frames1 = 2;
int example_lut1[num_frames1][LUT_PARAMS] = {
  {2500, 2500, 30, 1000},
  {2000, 2000, 30, 1000}
};


// TODO
// params: lut, info for struct
// returns: Animation struct
void newAnimation() {
  // TODO
}


// callbacks get processed here
void updateAnimation() {

  if(xSemaphoreTake(semaphore_anim_frame, 0) == pdTRUE) {
    callbackFrameDone();
  }

  if(xSemaphoreTake(semaphore_anim_loop, 0) == pdTRUE) {
    callbackAnimLoop();
  }

  if(xSemaphoreTake(semaphore_anim_done, 0) == pdTRUE) {
    callbackAnimDone();
  }

}


void initAnimations() {

  // --------------------------------------------

  const uint16_t flap_offset = 500;
  GentleFlap.id = 1;
  GentleFlap.frames = 2;
  GentleFlap.start = 0;
  GentleFlap.active = false;
  GentleFlap.index = 0;
  GentleFlap.loop = true;
  GentleFlap.reverse = false;
  GentleFlap.done = false;

  // frame 0: up (closed) to down (open)
  GentleFlap.servo_L[0] = SERVO_LEFT_HOME+flap_offset;
  GentleFlap.servo_R[0] = SERVO_RIGHT_HOME-flap_offset;
  GentleFlap.velocity[0] = 30;
  GentleFlap.dwell[0] = 100;
  
  // frame 1: down (open) to up (closed)
  GentleFlap.servo_L[1] = SERVO_LEFT_UP;
  GentleFlap.servo_R[1] = SERVO_RIGHT_UP;
  GentleFlap.velocity[1] = 30;
  GentleFlap.dwell[1] = 250;

  // --------------------------------------------

  HomeFrame.id = 2;
  HomeFrame.frames = 1;
  HomeFrame.start = 0;
  HomeFrame.active = false;
  HomeFrame.index = 0;
  HomeFrame.loop = false;
  HomeFrame.reverse = false;
  HomeFrame.done = false;

  HomeFrame.servo_L[0] = SERVO_LEFT_HOME; 
  HomeFrame.servo_R[0] = SERVO_RIGHT_HOME; 
  HomeFrame.velocity[0] = 30;
  HomeFrame.dwell[0] = 1000;
  
  // --------------------------------------------

}


// state: 1 = go, 0 = stop
bool sendAnimation(struct Animation *a, uint8_t state) {

  // take mutex prior to critical section
  if(xSemaphoreTake(Mutex_SM, (TickType_t)1000) == pdTRUE) {

    Serial << "sending animation" << endl;
    
    // -- critical selection
    PresentAnimation = *a;
    //memcpy(&PresentAnimation, a, sizeof(a));
    //      ^dest,            ^src,   ^size to copy

    ANIM_STATE = state;
    // --

    // give mutex after critical section
    xSemaphoreGive(Mutex_SM);

    // let the task know there's an update available
    xSemaphoreGive(semaphore_anim_new);

    return true;

  }

  return false;


  //Serial << "sending animation to queue" << endl;

  //struct Animation *structPtr = &GentleFlap; // this works

  // send it to the queue
  // time of 0 says don't block if the queue is already full
  if(DEBUG_ANIM) Serial << "sending animation to queue" << endl;
  //if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, (void *)&structPtr, (TickType_t)0); // this works
  if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, (void *)&a, (TickType_t)0);
  if(Queue_SM2 != NULL) xQueueSend(Queue_SM2, (void *)&state, (TickType_t)0);

}



