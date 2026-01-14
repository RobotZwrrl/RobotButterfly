void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);

  // show that it has connected
  setNeoAnim(&neo_animation_alert, NEO_ANIM_ZWOOP, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_OFF);
  setNeoAnimDuration(&neo_animation_alert, 1000);
  startNeoAnim(&neo_animation_alert);

  #if SENDER == 1  // the sender
    
    // start a neo animation
    startNeoHomeAnimation();

    // subscribe to topic: itself's-mqtt-id/sync
    robotbutterfly.mqttSubscribe(robotbutterfly.getPreference(SETTINGS_MQTT_ID)+"/sync");

    Serial << "started home neo animation and subscribed to sync" << endl;

  #else  // the receiver

    // subscribe to conductor robot
    robotbutterfly.mqttSubscribe(conductor_mqtt_id+"/neoanimation_home/#");
    robotbutterfly.mqttSubscribe(conductor_mqtt_id+"/neoanimation_alert/#");
    
    // publish a message to other robot / sync
    robotbutterfly.mqttPublish(conductor_mqtt_id+"/sync", "1");
  
  #endif

}


void myiotMessageReceivedCallback(String topic, String payload) {
  Serial << "--> iot received: " << topic << " = " << payload << endl;

  String topic_parts[MAX_SUBTOPICS];
  int parts_count = splitTopic(topic, topic_parts);
  for(uint8_t i=0; i<parts_count; i++) {
    Serial << "[" << i << "] " << topic_parts[i] << endl;
  }

  #if SENDER == 1  // the sender

    if(topic_parts[0] == robotbutterfly.getPreference(SETTINGS_MQTT_ID)) {
      Serial << "correct id" << endl;
      if(topic_parts[1] == "sync") {
        Serial << "topic is sync" << endl;

        int val = payload.toInt();
        if(val == 1) {
          syncNeoAnimation();
        }

      }
    } else {
      Serial << "incorrect id" << endl;
    }

  #else  // the receiver

    if(topic_parts[0] == conductor_mqtt_id) {
      if(topic_parts[1].startsWith("neoanimation")) {
        
        int type = -99;
        if(topic_parts[1] == "neoanimation_home") {
          type = NEO_ANIM_HOME;
        } else if(topic_parts[1] == "neoanimation_alert") {
          type = NEO_ANIM_ALERT;
        }

        // see if it's the /start or /stop topic
        bool y = checkStartStop(topic_parts[2], payload.toInt(), type);
        if(!y) {

          NeoAnimation neo_animation_temp;
          robotbutterfly.mqttParseNeoanimation(&neo_animation_temp, payload);
          
          if(neoanimation_temp->type == NEO_ANIM_HOME) {
            setNeoAnim(&neo_animation_home, neo_animation_temp.id, NEO_ANIM_HOME);
            copyNeoAnimations(&neoanimation_temp, &neo_animation_home);
          } else if(neoanimation_temp->type == NEO_ANIM_ALERT) {
            setNeoAnim(&neo_animation_alert, neo_animation_temp.id, NEO_ANIM_ALERT);
            copyNeoAnimations(&neo_animation_temp, &neo_animation_alert);
          }

        } else {

          if(topic_parts[1] == "neoanimation_home") {
            Serial << "parsing neoanimation_home" << endl;
            robotbutterfly.mqttParseNeoanimation(&neoanimation_home, payload);
          } else if(topic_parts[1] == "neoanimation_alert") {
            Serial << "parsing neoanimation_alert" << endl;
            robotbutterfly.mqttParseNeoanimation(&neoanimation_alert, payload);
          }
          
        }

      }
    }

  #endif

}


void myiotDisconnectedCallback() {
  Serial << "--> iot disconnected" << endl;
  digitalWrite(LED_COMMS_PIN, LOW);
  stopAllNeoAnimation();
}


void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}


