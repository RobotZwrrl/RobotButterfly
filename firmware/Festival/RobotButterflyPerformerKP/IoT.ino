void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);
  robotbutterfly.conductorSubscribe();
}

void myiotDisconnectedCallback() {
  Serial << "--> iot disconnected" << endl;
  digitalWrite(LED_COMMS_PIN, LOW);
}

void myiotMessageReceivedCallback(String topic, String payload) {
  Serial << "--> iot received: " << topic << " = " << payload << endl;
  uint8_t count = splitTopic(topic);
  uint8_t action = payload.toInt();

  if(topic_segments[count-1] == "control") {
    if(count == 4) { // team subtopic
      if(topic_segments[count-2] == robotbutterfly.getPreference(SETTINGS_TEAM) ) {
        
        Serial << "same team! action: " << action << endl;
        performAction(action);

      }
    }
  }
}

void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}


int splitTopic(String topic) {
  char delimiter = '/';
  int index = 0;
  int start = 0;

  for(int i=0; i<topic.length(); i++) {
    if(topic.charAt(i) == delimiter) {
      topic_segments[index++] = topic.substring(start, i);
      start = i + 1;
      if(index >= 7) break;
    }
  }
  if(index < 7 && start < topic.length()) {
    topic_segments[index++] = topic.substring(start);
  }

  return index;
}

