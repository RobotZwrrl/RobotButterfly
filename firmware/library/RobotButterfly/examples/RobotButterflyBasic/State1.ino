// ---------------------------
// --------- State 1 ---------
// ---------------------------

void setupState1() {
  if(new_enter) {
    // add your one-time init code here for state 1!
    Serial << "STATE " << RobotButterfly::STATE1 << " entrance" << endl;
    new_enter = false;
  }
  // add your looping init code here for state 1!
}

void loopState1() {
  if(new_update) {
    // add your one-time behaviour code here for state 1!
    new_update = false;
  }
  // add your looping behaviour code here for state 1!
  robotbutterfly.printStateHeartbeat(RobotButterfly::STATE1);
}

