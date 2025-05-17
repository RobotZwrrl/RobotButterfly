#include "RobotButterfly.h"

volatile bool new_enter = false;
volatile bool new_update = false;
volatile bool enter_state = false;
volatile bool update_state = false;
volatile bool new_print = false;

hw_timer_t* RobotButterfly::timer_state_cfg = NULL;

uint8_t RobotButterfly::CURRENT_STATE = 0;
uint8_t RobotButterfly::PREV_STATE = 0;
RobotButterfly::State* RobotButterfly::all_states[NUM_STATES] = { NULL };
bool RobotButterfly::CHANGE_STATES_CONTROL = true;
uint8_t RobotButterfly::MAX_STATES_COUNT = 0;

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


// init_servos refers to if initServos is called in this function
// this is useful in cases where checking for low power mode (eg, 
// on AA batts)
// state_machine refers to automatically using the buttons to
// increment / decrement the state. for simple sketches this might
// not be desired
void RobotButterfly::init(bool init_servos, bool state_machine) {
  
  initStateMachine();
  initButtons();
  initSound();
  initIMU();
  initNeopixels();
  initNeoAnimations();
  if(init_servos) {
    initServos(SERVO_MODE_INIT_BOTH);
    initServoAnimations();
  }
  initSensors();
  initProximity();

  CHANGE_STATES_CONTROL = state_machine;
  MAX_STATES_COUNT = 0;

  // -- button callbacks --
  onHoldNotificationCallback = buttonHoldNotificationCallback;
  onHoldReleasedCallback = buttonHoldReleasedCallback;
  onClickCallback = buttonClickCallback;
  onReleaseCallback = buttonReleaseCallback;

  onHoldNotificationCallback_client = NULL;
  onHoldReleasedCallback_client = NULL;
  onClickCallback_client = NULL;
  onReleaseCallback_client = NULL;
  // --

  // -- sound callbacks --
  onSoundDoneCallback = soundDoneCallback;

  onSoundDoneCallback_client = NULL;
  // --

  // -- imu callbacks --
  onStateChangeCallback = imuStateChangeCallback;
  onOrientationChangeCallback = imuOrientationChangeCallback;
  onPoseChangeCallback = imuPoseChangeCallback;
  onEventDetectedCallback = imuEventDetectedCallback;

  onStateChangeCallback_client = NULL;
  onOrientationChangeCallback_client = NULL;
  onPoseChangeCallback_client = NULL;
  onEventDetectedCallback_client = NULL;
  // --

  // -- neoanim callbacks --
  onNeoAnimDoneCallback = neoAnimDoneCallback;
  onNeoAnimLoopCallback = neoAnimLoopCallback;

  onNeoAnimDoneCallback_client = NULL;
  onNeoAnimLoopCallback_client = NULL;
  // --

  // -- servoanim callbacks --
  onServoAnimDoneCallback = servoAnimDoneCallback;
  onServoAnimLoopCallback = servoAnimLoopCallback;
    
  onServoAnimDoneCallback_client = NULL;
  onServoAnimLoopCallback_client = NULL;
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
  
  onSensorLightChangeCallback_client = NULL;
  onSensorLightAmbientChangeCallback_client = NULL;
  onSensorSoundChangeCallback_client = NULL;
  onSensorSoundAmbientChangeCallback_client = NULL;
  onSensorTemperatureChangeCallback_client = NULL;
  onSensorTemperatureAmbientChangeCallback_client = NULL;
  onSensorHumidityChangeCallback_client = NULL;
  onSensorHumidityAmbientChangeCallback_client = NULL;
  // --

  // -- proximity callbacks --
  ultrasonic.onProximityTriggerCallback = proximityTriggerCallback;
  
  onProximityTriggerCallback_client = NULL;
  // --

}


// only updates the state machine
// - good for when rtos mode is enabled
void RobotButterfly::update() {

  updateStateMachine();
  
}


// parameters use an enum (UpdateOptions) to choose 
// what updates with more granularity
void RobotButterfly::update(uint8_t update_statemachine, 
                            uint8_t update_buttons, 
                            uint8_t update_sound, 
                            uint8_t update_imu, 
                            uint8_t update_neoanim, 
                            uint8_t update_servoanim, 
                            uint8_t update_sensors, 
                            uint8_t update_proximity) {

  if(update_statemachine == UPDATE_STATEMACHINE_ON) updateStateMachine();
  if(update_buttons == UPDATE_BUTTONS_ON) updateButtons();
  if(update_sound == UPDATE_SOUND_ON) updateSound();
  if(update_imu == UPDATE_IMU_ON) updateIMU();
  if(update_neoanim == UPDATE_NEOANIM_ON) updateNeoAnimation();
  if(update_servoanim == UPDATE_SERVOANIM_ON) updateServoAnimation();
  if(update_sensors == UPDATE_SENSORS_ON) updateSensors();
  if(update_proximity == UPDATE_PROXIMITY_ON) updateProximity();
  
}


// ----------------------------------
// --------- state machine ----------
// ----------------------------------

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


void RobotButterfly::transitionState() {

  if(DEBUG_STATEMACHINE) Serial << "transitioning state" << endl;

  if(all_states[CURRENT_STATE] == NULL) return;
  if(all_states[PREV_STATE] == NULL) return;

  all_states[PREV_STATE]->enabled = false;
  all_states[CURRENT_STATE]->enabled = true;

  State *s = all_states[CURRENT_STATE];

  // start the transition state process 
  // (eg, to show the lights representing which state number)
  s->t_transition = millis();
  update_state = false;

  // clean up old timer
  if (timer_state_cfg) {
    timerAlarmDisable(timer_state_cfg);
    timerDetachInterrupt(timer_state_cfg);
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


void RobotButterfly::addState(uint8_t id, StateSetup setup_fn, StateLoop loop_fn) {
  if(id > NUM_STATES-1) return;
  all_states[id]->id = id;
  all_states[id]->setup_fn = setup_fn;
  all_states[id]->loop_fn = loop_fn;
  all_states[id]->enabled = true;
  MAX_STATES_COUNT++;
}


void RobotButterfly::changeState(uint8_t id) {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE = id;
  if(CURRENT_STATE >= MAX_STATES_COUNT) CURRENT_STATE = MAX_STATES_COUNT-1;
  transitionState();
}


void RobotButterfly::incrementState() {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE++;
  if(CURRENT_STATE >= MAX_STATES_COUNT) {
    CURRENT_STATE = 0; // loop around
  }
  transitionState();
}


void RobotButterfly::decrementState() {
  PREV_STATE = CURRENT_STATE;
  if(CURRENT_STATE == 0) {
    CURRENT_STATE = MAX_STATES_COUNT-1; // loop around
  } else {
    CURRENT_STATE--;
  }
  transitionState();
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

// ----------------------------------
// ----------------------------------


