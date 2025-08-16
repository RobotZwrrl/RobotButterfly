void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  digitalWrite(LED_COMMS_PIN, HIGH);
  
  // sync it up the 1st time!
  if(!initial_connection) {
    restAction();
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
