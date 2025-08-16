void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);
  
  // sync it up the 1st time!
  if(!initial_connection) {
    restAction();

    setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
    if(team == 1) { // red
      setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_RED);
    } else if(team == 2) { // blue
      setNeoAnimColours(&neo_animation_home, NEO_WHITE, NEO_BLUE);
    } else { // unknown
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
}

void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}
