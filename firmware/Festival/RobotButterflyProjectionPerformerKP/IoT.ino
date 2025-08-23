void myiotMessageReceivedCallback(String topic, String payload) {
  Serial << "--> iot received: " << topic << " = " << payload << endl;
  uint8_t count = splitTopic(topic);
  uint8_t action = payload.toInt();

  if(checkIndividualControl(count)) {
    Serial << "mqtt name match! action: " << action << endl;
    performIndividualAction(action);
  }

  if(checkTeamControl(count)) {
    Serial << "mqtt team match! action: " << action << endl;
    performTeamAction(action);
  }
}


bool checkIndividualControl(uint8_t count) {
  if(count != 2) return false;
  if(topic_segments[0] == robotbutterfly.getPreference(SETTINGS_MQTT_ID)) {
    if(topic_segments[1] == "control") {
      return true;
    }
  }
  return false;
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


void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);
  robotbutterfly.conductorSubscribe();

  String topic = ( robotbutterfly.getPreference(SETTINGS_MQTT_ID) + "/control" );
  robotbutterfly.mqttSubscribe( topic );
}


void myiotDisconnectedCallback() {
  Serial << "--> iot disconnected" << endl;
  digitalWrite(LED_COMMS_PIN, LOW);
}


void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}

