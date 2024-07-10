
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


void initAnimations() {

  // --------------------------------------------

  const uint16_t flap_offset = 500;
  GentleFlap.id = 0;
  GentleFlap.frames = 2;
  GentleFlap.start = 0;
  GentleFlap.active = false;
  GentleFlap.index = 0;
  GentleFlap.loop = true;
  GentleFlap.reverse = false;

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
  HomeFrame.loop = true;
  HomeFrame.reverse = false;

  HomeFrame.servo_L[0] = SERVO_LEFT_HOME; 
  HomeFrame.servo_R[0] = SERVO_RIGHT_HOME; 
  HomeFrame.velocity[0] = 30;
  HomeFrame.dwell[0] = 1000;
  
  // --------------------------------------------

}


void sendAnimation(struct Animation *a, uint8_t state) {

  //struct Animation *structPtr = &GentleFlap; // this works

  // send it to the queue
  // time of 0 says don't block if the queue is already full
  if(DEBUG_ANIM) Serial << "sending animation to queue" << endl;
  //if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, (void *)&structPtr, (TickType_t)0); // this works
  if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, (void *)&a, (TickType_t)0);
  if(Queue_SM2 != NULL) xQueueSend(Queue_SM2, (void *)&state, (TickType_t)0);

}


/*
void loadAnimation(struct Animation *a, int[][] lut, uint8_t steps) {

  struct Keyframe f;

  f.servo_L = lut[index][0];
  f.servo_R = lut[index][1];
  f.velocity = lut[index][2];
  f.dwell = lut[index][3];

  


  

  // reset the animation first
  if(DEBUG_ANIM) Serial << "resetting animation" << endl;
  for(uint8_t i=0; i<ANIM_MAX; i++) {
    Animation[i].servo_L = 9999;
    Animation[i].servo_R = 9999;
    Animation[i].velocity = 0;
    Animation[i].dwell = 0;
    Animation[i].start = 0;
    Animation[i].moving = false;
  }

  // load the animation
  if(DEBUG_ANIM) Serial << "loading animation" << endl;
  for(uint8_t i=0; i<ANIM_MAX; i++) {
    if(i>=steps) break;
    Animation[i].servo_L = a[i].servo_L;
    Animation[i].servo_R = a[i].servo_R;
    Animation[i].velocity = a[i].velocity;
    Animation[i].dwell = a[i].dwell;
    Animation[i].start = a[i].start;
    Animation[i].moving = a[i].moving;
  }

  Serial << "dwell time loaded: " << Animation[0].dwell << endl;

  // send it to the queue
  // time of 0 says don't block if the queue is already full
  if(DEBUG_ANIM) Serial << "sending animation to queue" << endl;
  if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, &a, (TickType_t)0);
  
}
*/




/*
void animationGentleFlap() {

  // dwell wait at the end of the frame
  if(millis()-FlapFrames[flap_index].start >= FlapFrames[flap_index].dwell) {
    // done! move to next frame
    if(DEBUG_ANIM) Serial << "Animation done dwell" << endl;
    FlapFrames[flap_index].moving = false;
    flap_index++;
    if(flap_index >= flap_steps) {
      flap_index = 0;
    }
  }

  if(FlapFrames[flap_index].moving == false) {

    if(DEBUG_ANIM) Serial << "Animation frame " << flap_index << " start" << endl;

    if(FlapFrames[flap_index].servo_L != 9999) {
      wing_left.current_pos = FlapFrames[flap_index].servo_L;
    }

    if(FlapFrames[flap_index].servo_R != 9999) {
      wing_right.current_pos = FlapFrames[flap_index].servo_R;
    }

    FlapFrames[flap_index].moving = true;
    FlapFrames[flap_index].start = millis();
    wing_left.motor.startEaseTo(wing_left.current_pos, FlapFrames[flap_index].velocity);
    wing_right.motor.startEaseTo(wing_right.current_pos, FlapFrames[flap_index].velocity);

  }

}
*/
