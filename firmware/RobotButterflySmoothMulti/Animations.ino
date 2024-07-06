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


void initAnimations() {

  // --------------------------------------------

  uint16_t flap_offset = 500;

  // frame 0: up (closed) to down (open)
  FlapFrames[0].servo_L = SERVO_LEFT_HOME+flap_offset; 
  FlapFrames[0].servo_R = SERVO_RIGHT_HOME-flap_offset; 
  FlapFrames[0].velocity = 30;
  FlapFrames[0].dwell = 100;
  FlapFrames[0].moving = false;

  // frame 1: down (open) to up (closed)
  FlapFrames[1].servo_L = SERVO_LEFT_UP; 
  FlapFrames[1].servo_R = SERVO_RIGHT_UP; 
  FlapFrames[1].velocity = 30;
  FlapFrames[1].dwell = 250;
  FlapFrames[1].moving = false;

  // --------------------------------------------

  HomeFrames[0].servo_L = SERVO_LEFT_HOME; 
  HomeFrames[0].servo_R = SERVO_RIGHT_HOME; 
  HomeFrames[0].velocity = 30;
  HomeFrames[0].dwell = 1000;
  HomeFrames[0].moving = false;

  // --------------------------------------------

}


void loadAnimation(struct Keyframe *a, uint8_t steps, uint8_t index) {

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
  // TODO: check if queue is null
  if(DEBUG_ANIM) Serial << "sending animation to queue" << endl;
  if(Queue_SM1 != NULL) xQueueSend(Queue_SM1, &Animation, (TickType_t)0);
  if(Queue_SM2 != NULL) xQueueSend(Queue_SM2, &steps, (TickType_t)0);
  if(Queue_SM3 != NULL) xQueueSend(Queue_SM3, &index, (TickType_t)0);

}


