void myiotConnectedCallback() {
  Serial << "--> iot connected" << endl;
  robotbutterfly.conductorSubscribe();
}

void myiotDisconnectedCallback() {
  Serial << "--> iot disconnected" << endl;
}

void myiotMessageReceivedCallback(String topic, String payload) {
  Serial << "--> iot received: " << topic << " = " << payload << endl;
}

void myiotMessagePublishedCallback(String topic, String payload) {
  Serial << "--> iot published: " << topic << " = " << payload << endl;
}

