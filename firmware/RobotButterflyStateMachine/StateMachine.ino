void updateStateMachine() {

  // state entrance
  if(enter_state) {

    switch(CURRENT_STATE) {
      case STATE1:
        setupState1();
      break;
      case STATE2:
        setupState2();
      break;
      case STATE3:
        setupState3();
      break;
      case STATE4:
        setupState4();
      break;
      case STATE5:
        setupState5();
      break;
      case STATE6:
        setupState6();
      break;
      case STATE7:
        setupState7();
      break;
      case STATE8:
        setupState8();
      break;
    }

  }

  
  // state machine loop
  if(update_state) {

    t_enter = millis();
    t_delta = t_enter - t_transition;
    //if(DEBUG_STATEMACHINE) Serial << "delta: " << t_delta << " enter (" << t_enter << ") - transition (" << t_transition << ")" << endl;

    // stop the timer
    //if(DEBUG_STATEMACHINE) Serial << "timer done" << endl;
    //timerEnd(timer_state_cfg);

    switch(CURRENT_STATE) {
      case STATE1:
        loopState1();
      break;
      case STATE2:
        loopState2();
      break;
      case STATE3:
        loopState3();
      break;
      case STATE4:
        loopState4();
      break;
      case STATE5:
        loopState5();
      break;
      case STATE6:
        loopState6();
      break;
      case STATE7:
        loopState7();
      break;
      case STATE8:
        loopState8();
      break;
    }
  }

}


void printStateHeartbeat(int s) {

  if(millis()-last_state_print >= STATE_LOOP_PRINT || new_print == true) {
    Serial << "STATE " << s << " loop" << endl;
    new_print = false;
    last_state_print = millis();
  }

}


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

  if(DEBUG_STATEMACHINE) Serial << "transitioning state" << endl;

  // start the transition state process 
  // (eg, to show the lights representing which state number)
  t_transition = millis();
  update_state = false;

  // timer transition - timer 3
  timer_state_cfg = timerBegin(3, 8000, true);
  timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);
  // params: timer, tick count, auto-reload (false to run once)
  timerAlarmWrite(timer_state_cfg, TRANSITION_FRAME_TIME, false);
  timerAlarmEnable(timer_state_cfg);
  enter_state = true;
  new_enter = true;
  
  last_state_change = millis();
  if(DEBUG_STATEMACHINE) Serial << "entering state index: " << CURRENT_STATE << " from index: " << PREV_STATE << endl;
}


void initStateMachine() {

  // timer transition - timer 3
  timer_state_cfg = timerBegin(3, 8000, true);
  timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);

}

