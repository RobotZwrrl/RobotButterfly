void myNeoAnimStartCallback(struct NeoAnimation *a) {

  /*
  #if SENDER == 1

    if(a->id == NEO_ANIM_NONE) return;

    robotbutterfly.mqttPublishNeoanimation(a);

    String topic = "";
    if(a->type == NEO_ANIM_HOME) {
      topic = "neoanimation_home/start";
    } else if(a->type == NEO_ANIM_ALERT) {
      topic = "neoanimation_alert/start";
    }

    if(topic != "") robotbutterfly.mqttPublish(topic, "1");

  #endif
  */

}


void myNeoAnimDoneCallback(struct NeoAnimation *a) {

  #if SENDER == 1

    if(a->id == NEO_ANIM_NONE) return;

    if(a->num_repeats == -99) return; // loops indefinitely

    Serial << "the anim is done" << endl;

    String topic = "";
    if(a->type == NEO_ANIM_HOME) {
      topic = "neoanimation_home/stop";
    } else if(a->type == NEO_ANIM_ALERT) {
      topic = "neoanimation_alert/stop";
    }

    if(topic != "") robotbutterfly.mqttPublish(topic, "1");

  #endif

}


void myNeoAnimLoopCallback(struct NeoAnimation *a) {

  #if SENDER == 1

    if(a->id == NEO_ANIM_NONE) return;

    // re-sending to help keep it in sync
    robotbutterfly.mqttPublishNeoanimation(a);

    String topic = "";
    if(a->type == NEO_ANIM_HOME) {
      topic = "neoanimation_home/start";
    } else if(a->type == NEO_ANIM_ALERT) {
      topic = "neoanimation_alert/start";
    }

    if(topic != "") robotbutterfly.mqttPublish(topic, "1");

  #endif

}

