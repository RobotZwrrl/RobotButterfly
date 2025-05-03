#include "RobotButterfly.h"

volatile bool new_enter = false;
volatile bool new_update = false;
volatile bool enter_state = false;
volatile bool update_state = false;
volatile bool new_print = false;

uint8_t RobotButterfly::CURRENT_STATE = 0;
uint8_t RobotButterfly::PREV_STATE = 0;
RobotButterfly::State* RobotButterfly::all_states[NUM_STATES] = { NULL };
hw_timer_t* RobotButterfly::timer_state_cfg = NULL;

RobotButterfly::State RobotButterfly::state1;
RobotButterfly::State RobotButterfly::state2;
RobotButterfly::State RobotButterfly::state3;
RobotButterfly::State RobotButterfly::state4;
RobotButterfly::State RobotButterfly::state5;
RobotButterfly::State RobotButterfly::state6;
RobotButterfly::State RobotButterfly::state7;
RobotButterfly::State RobotButterfly::state8;

void IRAM_ATTR Timer_State_ISR() {
  enter_state = false;
  update_state = true;
  new_print = true;
  new_update = true;
}


RobotButterfly::RobotButterfly() {
}

void RobotButterfly::init() {
  
  initStateMachine();

  initButtons();
  initSound();
  // initIMU();
  // initNeopixels();
  // initNeoAnimations();
  // initServos(SERVO_MODE_INIT_BOTH);
  // initServoAnimations();
  // initSensors();
  initProximity();


  // -- button callbacks --
  onHoldNotificationCallback = buttonHoldNotificationCallback;
  onHoldReleasedCallback = buttonHoldReleasedCallback;
  onClickCallback = buttonClickCallback;
  onReleaseCallback = buttonReleaseCallback;
  // --

  // -- sound callbacks --
  onSoundDoneCallback = soundDoneCallback;
  // --

  // -- imu callbacks --
  onStateChangeCallback = imuStateChangeCallback;
  onOrientationChangeCallback = imuOrientationChangeCallback;
  onPoseChangeCallback = imuPoseChangeCallback;
  onEventDetectedCallback = imuEventDetectedCallback;
  // --

  // -- neoanim callbacks --
  onNeoAnimDoneCallback = neoAnimDoneCallback;
  onNeoAnimLoopCallback = neoAnimLoopCallback;
  // --

  // -- sensor callbacks --
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorChangeCallback = sensorHumidityChangeCallback;
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorAmbientChangeCallback = sensorHumidityAmbientChangeCallback;

  all_sensors[SENSOR_ID_LIGHT]->onSensorChangeCallback = sensorLightChangeCallback;
  all_sensors[SENSOR_ID_LIGHT]->onSensorAmbientChangeCallback = sensorLightAmbientChangeCallback;

  all_sensors[SENSOR_ID_SOUND]->onSensorChangeCallback = sensorSoundChangeCallback;
  all_sensors[SENSOR_ID_SOUND]->onSensorAmbientChangeCallback = sensorSoundAmbientChangeCallback;

  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorChangeCallback = sensorTemperatureChangeCallback;
  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorAmbientChangeCallback = sensorTemperatureAmbientChangeCallback;
  // --

  // -- proximity callbacks --
  ultrasonic.onProximityTriggerCallback = proximityTriggerCallback;
  // --

  // neoanim testing
  // setNeoAnim(&neo_animation_home, NEO_ANIM_POLKADOT, NEO_ANIM_HOME);
  // setNeoAnimColours(&neo_animation_home, NEO_MAGENTA, NEO_CYAN);
  // startNeoAnim(&neo_animation_home);

  // servoanim testing
  // setServoAnim(&servo_animation_alert, SERVO_ANIM_SWAY, SERVO_ANIM_ALERT);
  // startServoAnim(&servo_animation_alert);

  // sensor testing
  // for(uint8_t i=0; i<NUM_SENSORS; i++) {
  //   if(all_sensors[i]) all_sensors[i]->print = true;
  // }

  playSound(SOUND_ALERT_STARTUP);

}


void RobotButterfly::update() {

  updateStateMachine();

  updateButtons();
  updateSound();
  // updateIMU();
  // updateNeoAnimation();
  // updateServoAnimation();
  // updateSensors();
  updateProximity();

}


void RobotButterfly::updateStateMachine() {

  State *s = all_states[CURRENT_STATE];
  if(s == NULL) return;

  // state entrance
  if(enter_state) {
    
    if(s->enabled) {
      if(s->setup_fn) s->setup_fn();
    }

  }
  
  // state loop
  if(update_state) {

    s->t_enter = millis();
    s->t_delta = s->t_enter - s->t_transition;
    //if(DEBUG_STATEMACHINE) Serial << "delta: " << s.t_delta << " enter (" << s.t_enter << ") - transition (" << s.t_transition << ")" << endl;

    if(s->enabled) {
      if(s->loop_fn) s->loop_fn();
    }
    
  }

}


void RobotButterfly::addState(uint8_t id, StateSetup setup_fn, StateLoop loop_fn) {
  all_states[id]->id = id;
  all_states[id]->setup_fn = setup_fn;
  all_states[id]->loop_fn = loop_fn;
  all_states[id]->enabled = true;
}


void RobotButterfly::printStateHeartbeat(uint8_t id) {

  State *s = all_states[id];
  if(s == NULL) return;

  if(millis()-s->last_state_print >= STATE_LOOP_PRINT || s->new_print == true) {
    Serial << "STATE " << id << " loop" << endl;
    s->new_print = false;
    s->last_state_print = millis();
  }

}


void RobotButterfly::incrementState() {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE++;
  if(CURRENT_STATE >= MAX_STATE) {
    CURRENT_STATE = 0; // loop around
  }
  transitionState();
}


void RobotButterfly::decrementState() {
  PREV_STATE = CURRENT_STATE;
  if(CURRENT_STATE == 0) {
    CURRENT_STATE = MAX_STATE-1; // loop around
  } else {
    CURRENT_STATE--;
  }
  transitionState();
}


void RobotButterfly::changeState(uint8_t id) {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE = id;
  if(CURRENT_STATE >= MAX_STATE) CURRENT_STATE = MAX_STATE-1;
  transitionState();
}


void RobotButterfly::transitionState() {

  if(DEBUG_STATEMACHINE) Serial << "transitioning state" << endl;

  if(all_states[CURRENT_STATE] == NULL) return;
  if(all_states[PREV_STATE] == NULL) return;

  if(DEBUG_STATEMACHINE) Serial << "transitioning state" << endl;

  all_states[PREV_STATE]->enabled = false;
  all_states[CURRENT_STATE]->enabled = true;

  State *s = all_states[CURRENT_STATE];
  if(s == NULL) return;

  // start the transition state process 
  // (eg, to show the lights representing which state number)
  s->t_transition = millis();
  update_state = false;

  // clean up old timer
  if (timer_state_cfg) {
    timerEnd(timer_state_cfg);
    timer_state_cfg = NULL;
  }

  // timer transition - timer 0
  timer_state_cfg = timerBegin(0, 8000, true);
  timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);
  // params: timer, tick count, auto-reload (false to run once)
  timerAlarmWrite(timer_state_cfg, TRANSITION_FRAME_TIME, false);
  timerAlarmEnable(timer_state_cfg);
  enter_state = true;
  new_enter = true;

  s->last_state_change = millis();
  if(DEBUG_STATEMACHINE) Serial << "entering state index: " << CURRENT_STATE << " from index: " << PREV_STATE << endl;
}


void RobotButterfly::initStateMachine() {

  enter_state = false;
  update_state = false;
  new_print = false;
  new_enter = false;
  new_update = false;

  // -- init state machine --
  all_states[STATE1] = &state1;
  all_states[STATE2] = &state2;
  all_states[STATE3] = &state3;
  all_states[STATE4] = &state4;
  all_states[STATE5] = &state5;
  all_states[STATE6] = &state6;
  all_states[STATE7] = &state7;
  all_states[STATE8] = &state8;
  // --

  // timer transition - timer 0
  //timer_state_cfg = timerBegin(0, 8000, true);
  //timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);

}









// ----------------------------------
// --------- button callbacks -------
// ----------------------------------

// give user feedback that they have held the
// button and its time to to release the button
void RobotButterfly::buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      playSimpleTone(NOTE_F5, 500);
      playNoTone();
    break;
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 500);
      playNoTone();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 500);
      playNoTone();
    break;
  }
  // TODO: call user callback
}

// do an action here
void RobotButterfly::buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
    break;
    case BUTTON_LEFT:
    break;
    case BUTTON_RIGHT:
    break;
  }
  // TODO: call user callback
}

// do an action here
void RobotButterfly::buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      incrementState();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      decrementState();
    break;
  }
  // TODO: call user callback
}

// probably not necessary to do anything here
void RobotButterfly::buttonReleaseCallback(uint8_t n) {
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// --------- sound callbacks --------
// ----------------------------------

void RobotButterfly::soundDoneCallback(uint8_t id) {
  if(DEBUG_SOUND) Serial << "done sound (" << id << ")" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ---------- imu callbacks ---------
// ----------------------------------

void RobotButterfly::imuStateChangeCallback(uint8_t s) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu state changed" << endl;
  // do actions here
  switch(s) {
    case IMU_SETTLE:
    break;
    case IMU_CALIBRATE_HOME:
    break;
    case IMU_ACTIVE:
    break;
    case IMU_INACTIVE:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuOrientationChangeCallback(uint8_t o) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu orientation changed" << endl;
  // do actions here
  switch(o) {
    case IMU_TABLETOP:
    break;
    case IMU_HANG:
    break;
    case IMU_UNKNOWN:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuPoseChangeCallback(uint8_t p) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu pose changed" << endl;
  // do actions here
  switch(p) {
    case IMU_Pose_Tilt_L:
    break;
    case IMU_Pose_Tilt_R:
    break;
    case IMU_Pose_Tilt_Fwd:
    break;
    case IMU_Pose_Tilt_Bwd:
    break;
    case IMU_Pose_Home:
    break;
    case IMU_Pose_NA:
    break;
  }
  // TODO: call user callback
}

void RobotButterfly::imuEventDetectedCallback(uint8_t e) {
  if(DEBUG_IMU_NEWS) Serial << millis() << " imu event detected" << endl;
  // do actions here
  if(e) {
    // event detected
  }
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// -------- neoanim callbacks -------
// ----------------------------------

// the neo animation is done entirely
void RobotButterfly::neoAnimDoneCallback(struct NeoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") done" << endl;
  // TODO: call user callback
}

// the neo animation is done a loop
void RobotButterfly::neoAnimLoopCallback(struct NeoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == NEO_ANIM_HOME) return;
  Serial << "Callback: Neo animation (" << a->id << ") looped" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ------ servoanim callbacks -------
// ----------------------------------

// the servo animation is done
void RobotButterfly::servoAnimDoneCallback(struct ServoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == SERVO_ANIM_HOME) return;
  Serial << "Callback: Servo animation (" << a->id << ") done" << endl;
  // TODO: call user callback
}

// the servo animation has looped
void RobotButterfly::servoAnimLoopCallback(struct ServoAnimation *a) {
  // TODO: remember to get rid of the line below
  if(a->type == SERVO_ANIM_HOME) return;
  if(a->num_frames <= 1) return; // skip the one frame animations
  Serial << "Callback: Servo animation (" << a->id << ") looped" << endl;
  // TODO: call user callback
}

// ----------------------------------


// ----------------------------------
// ------- sensor callbacks ---------
// ----------------------------------

void RobotButterfly::sensorLightChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Light on!";
  } else {
    Serial << "---> Light off!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorLightAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Light ambient brighter!" << endl;
  } else {
    Serial << "---> Light ambient darker!" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorSoundChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Sound louder!";
  } else {
    Serial << "---> Sound quieter!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorSoundAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Sound ambient louder!" << endl;
  } else {
    Serial << "---> Sound ambient quieter!" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == false) {
    Serial << "---> Temperature warmer!";
  } else {
    Serial << "---> Temperature colder!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change < 0) {
    Serial << "---> Temperature ambient warmer" << endl;
  } else {
    Serial << "---> Temperature ambient colder" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorHumidityChangeCallback(struct Sensor *s, bool trigger_dir) {

  // TODO: remember to get rid of the lines below

  if(trigger_dir == true) {
    Serial << "---> Humidity decrease!";
  } else {
    Serial << "---> Humidity increase!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

  // TODO: call user callback

}

void RobotButterfly::sensorHumidityAmbientChangeCallback(struct Sensor *s, int change) {
  
  // TODO: remember to get rid of the lines below

  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << endl;

  if(change > 0) {
    Serial << "---> Humidity ambient decrease" << endl;
  } else {
    Serial << "---> Humidity ambient increase" << endl;
  }

  Serial << "\r\n\r\n" << endl;

  // TODO: call user callback

}

// ----------------------------------


// ----------------------------------
// ----- proximity callbacks --------
// ----------------------------------

// uses the raw value to trigger the close proximity
// this is called at intervals defined by PROXIMITY_TRIGGER_FREQ
void RobotButterfly::proximityTriggerCallback(struct Proximity *p) {
  
  // TODO: remember to get rid of the lines below
  
  Serial << "----> Close proximity detected!" << endl;
  
  // TODO: call user callback

}

// ----------------------------------


