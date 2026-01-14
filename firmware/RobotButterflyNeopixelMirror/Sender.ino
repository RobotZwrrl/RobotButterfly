void syncNeoAnimation() {
  
  // 1. save a temp copy of the neoanimations
  NeoAnimation temp_neo_animation_home;
  NeoAnimation temp_neo_animation_alert;

  copyNeoAnimations(&neo_animation_home, &temp_neo_animation_home); // source, destination
  copyNeoAnimations(&neo_animation_alert, &temp_neo_animation_alert); // source, destination

  // 2. stop all neoanimations
  stopAllNeoAnimation();
  robotbutterfly.mqttPublish("neoanimation_home/stop", "1");
  robotbutterfly.mqttPublish("neoanimation_alert/stop", "1");
  
  // 3. start the neoanimations
  // if the animation was none, don't bother to send
  if(temp_neo_animation_home.id != NEO_ANIM_NONE
  && temp_neo_animation_home.active == false) {
    robotbutterfly.mqttPublish("neoanimation_home/start", "1");
    setNeoAnim(&neo_animation_home, temp_neo_animation_home.id, NEO_ANIM_HOME);
    copyNeoAnimations(&temp_neo_animation_home, &neo_animation_home); // source, destination
    startNeoAnim(&neo_animation_home);
  }

  // if the animation was none, don't bother to send
  if(temp_neo_animation_alert.id != NEO_ANIM_NONE
  && temp_neo_animation_alert.active == false) {
    robotbutterfly.mqttPublish("neoanimation_alert/start", "1");
    setNeoAnim(&neo_animation_alert, temp_neo_animation_alert.id, NEO_ANIM_ALERT);
    copyNeoAnimations(&temp_neo_animation_alert, &neo_animation_alert); // source, destination
    startNeoAnim(&neo_animation_alert);
  }

}


void startNeoHomeAnimation() {
  // start the neoanimation
  // the callback will send it over mqtt
  setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
  setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_ORANGE);
  setNeoAnimSpeed(&neo_animation_home, 100);
  startNeoAnim(&neo_animation_home);
  publishNeoAnimStart(&neo_animation_home);
}


void stopAllNeoAnimation() {
  setNeoAnim(&neo_animation_home, NEO_ANIM_NONE, NEO_ANIM_HOME);
  startNeoAnim(&neo_animation_home);
  setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
  startNeoAnim(&neo_animation_alert);
}


void publishNeoAnimStart(struct NeoAnimation *a) {

  if(a->id == NEO_ANIM_NONE) return;

  robotbutterfly.mqttPublishNeoanimation(a);

  String topic = "";
  if(a->type == NEO_ANIM_HOME) {
    topic = "neoanimation_home/start";
  } else if(a->type == NEO_ANIM_ALERT) {
    topic = "neoanimation_alert/start";
  }

  if(topic != "") robotbutterfly.mqttPublish(topic, "1");

}



