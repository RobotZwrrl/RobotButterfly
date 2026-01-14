
bool checkStartStop(String s, int val, int type) {
  bool y = false;
  if(s == "start") {
    if(val == 1) {
      y = true;
      startMirrorNeoAnimation(type);
    }
  } else if(s == "stop") {
    if(val == 1) {
      y = true;
      stopMirrorNeoAnimation(type);
    }
  }
  return y;
}


void startMirrorNeoAnimation(int type) {
  // the neo anim parameters have already been set
  if(type == NEO_ANIM_HOME) {
    startNeoAnim(&neo_animation_home);
  } else if(type == NEO_ANIM_ALERT) {
    startNeoAnim(&neo_animation_alert);
  }
}

void stopMirrorNeoAnimation(int type) {
  if(type == NEO_ANIM_HOME) {
    setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
    startNeoAnim(&neo_animation_home);
  } else if(type == NEO_ANIM_ALERT) {
    setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
    startNeoAnim(&neo_animation_alert);
  }
}

