void myonClickCallback(uint8_t b) {
  
  #if SENDER == 1
  
    if(!isIotConnected()) return; // don't animate if not connected

    if(b == BUTTON_LEFT) {
      setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
      setNeoAnimColours(&neo_animation_alert, NEO_RED, NEO_OFF);
      setNeoAnimDuration(&neo_animation_alert, 3000);
      startNeoAnim(&neo_animation_alert);
      publishNeoAnimStart(&neo_animation_alert);
    } else if(b == BUTTON_RIGHT) {
      setNeoAnim(&neo_animation_alert, NEO_ANIM_POLKADOT, NEO_ANIM_ALERT);
      setNeoAnimColours(&neo_animation_alert, NEO_BLUE, NEO_OFF);
      setNeoAnimDuration(&neo_animation_alert, 3000);
      startNeoAnim(&neo_animation_alert);
      publishNeoAnimStart(&neo_animation_alert);
    }

  #endif

}

