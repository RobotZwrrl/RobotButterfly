
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


void transitionState() {

  if(DEBUG_STATEMACHINE) Serial << millis() << " transition state" << endl;

  // start the transition state process (eg, to show the lights representing which state number)
  update_state = false;
  transition_state = true;

  timerAlarm(timer1_cfg, TRANSITION_FRAME_TIME, false, 0);
  timerRestart(timer1_cfg);
  timerStart(timer1_cfg);
  
  last_state_change = millis();
  if(DEBUG_STATEMACHINE) Serial << "entering state index: " << CURRENT_STATE << " from index: " << PREV_STATE << endl;
}

