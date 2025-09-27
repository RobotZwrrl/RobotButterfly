void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);
  robotbutterfly.conductorSubscribe();
  
  // sync it up the 1st time!
  if(!initial_connection) {
    restAction();

    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    if(team == RED_TEAM) {
      setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_RED);
    } else if(team == BLUE_TEAM) {
      setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_BLUE);
    } else if(team == OTHER_TEAM) {
      setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_GREEN);
    }
    setNeoAnimDuration(&neo_animation_home, 500);
    startNeoAnim(&neo_animation_home);

    initial_connection = true;
  }

}


void myiotDisconnectedCallback() {
  Serial << "--> iot disconnected" << endl;
  digitalWrite(LED_COMMS_PIN, LOW);
}


void myiotMessageReceivedCallback(String topic, String payload) {
  Serial << "--> iot received: " << topic << " = " << payload << endl;
  uint8_t count = splitTopic(topic);
  uint8_t action = payload.toInt();

  if(checkTeamControl(count)) {
    Serial << "mqtt team match! action: " << action << endl;
    performTeamAction(action);
  }
}


void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}


bool checkTeamControl(uint8_t count) {
  if(count != 4) return false;
  if(topic_segments[0] == robotbutterfly.getPreference(SETTINGS_NAMESPACE)) {
      if(topic_segments[1] == robotbutterfly.getPreference(SETTINGS_SET)) {
        if(topic_segments[2] == robotbutterfly.getPreference(SETTINGS_TEAM)) {
          if(topic_segments[3] == "control") {
            return true;
          }
        }
      }
    }
  return false;
}


int splitTopic(String topic) {
  char delimiter = '/';
  int index = 0;
  int start = 0;

  for(int i=0; i<topic.length(); i++) {
    if(topic.charAt(i) == delimiter) {
      topic_segments[index++] = topic.substring(start, i);
      start = i + 1;
      if(index >= MAX_SEGMENTS) break;
    }
  }
  if(index < MAX_SEGMENTS && start < topic.length()) {
    topic_segments[index++] = topic.substring(start);
  }

  return index;
}


