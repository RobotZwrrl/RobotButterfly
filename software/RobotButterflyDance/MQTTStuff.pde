void mqttConnect() {
 
  MQTT_credentials();
  mqtt_broker_url = "mqtt://" + mqtt_username + ":" + mqtt_password + "@" + mqtt_broker_ip + ":" + mqtt_broker_port;
  
  while(!mqtt_connected && connection_attempts < 5) {
    try {
      println("Attempt " + (connection_attempts+1) + " connecting to " + mqtt_broker_url);
      client.connect(mqtt_broker_url, mqtt_client_id);
      mqtt_connected = true;
      println("Connected!");
    } catch (Exception e) {
      println("Connection failed: " + e.getMessage());
      delay(2000); // wait before retry
      connection_attempts++;
    }
  }
  
  if(!mqtt_connected) {
    println("Could not connect after " + connection_attempts + " attempts.");
  }
  
}


void clientConnected() {
  println("Connected to MQTT broker!");
}


void messageReceived(String topic, byte[] payload) {
  println("Message received on topic " + topic + ": " + new String(payload));
}


void connectionLost() {
  println("Connection to MQTT broker lost.");
  mqttConnect();
}




