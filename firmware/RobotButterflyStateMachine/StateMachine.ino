
void incrementState() {

  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE++;
  if(CURRENT_STATE >= MAX_STATE) {
    CURRENT_STATE = 0; // loop around
  }

  transitionState();
}


void decrementState() {

  PREV_STATE = CURRENT_STATE;
  
  if(CURRENT_STATE == 0) {
    CURRENT_STATE = MAX_STATE-1; // loop around
  } else {
    CURRENT_STATE--;
  }
  
  transitionState();
}


void changeState(uint8_t n) {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE = n;
  if(CURRENT_STATE >= MAX_STATE) CURRENT_STATE = MAX_STATE-1;
  transitionState();
}


void transitionState() {

  if(DEBUG_STATEMACHINE) Serial << millis() << " transition state" << endl;

  // start the transition state process (eg, to show the lights representing which state number)
  t_transition = millis();
  if(DEBUG_STATEMACHINE) Serial << "transition: " << t_transition << endl;
  update_state = false;

  initStateMachine();
  
  last_state_change = millis();
  if(DEBUG_STATEMACHINE) Serial << "entering state index: " << CURRENT_STATE << " from index: " << PREV_STATE << endl;
}


void initStateMachine() {

  if(TRANSITION_FRAME_TIME > 0) { // don't create if transition time is 0
    // timer transition - timer 3
    timer_state_cfg = timerBegin(3, 8000, true);
    timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);
    // params: timer, tick count, auto-reload (false to run once)
    timerAlarmWrite(timer_state_cfg, TRANSITION_FRAME_TIME, false);
    timerAlarmEnable(timer_state_cfg);
    //timerStop(timer_state_cfg);
  }

  enter_state = true;

}

