void updateWifi() {

  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_COMMS_PIN, HIGH);
    if(WIFI_CONNECTING) {
      WIFI_CONNECTING = false;
      if(DEBUG_WIFI) Serial << endl;
      if(DEBUG_WIFI) Serial << "connected to wifi " << WiFi.localIP() << endl;
    }
    return;
  }

  if(WIFI_CONNECTING) {

    if(millis()-last_print >= 1000) {
      Serial << ".";
      last_print = millis();
    }

    if(millis()-connect_time >= WIFI_CONNECTION_TIMEOUT) {
      if(DEBUG_WIFI) Serial << "connection timed out" << endl;
      if(DEBUG_WIFI) Serial << "did not connect to wifi" << endl;
      digitalWrite(LED_COMMS_PIN, LOW);
      WIFI_CONNECTING = false;
      return;
    }

  }

  // auto reconnect when not already connecting
  if(AUTO_RECONNECT_WIFI == true && WIFI_CONNECTING == false) {
    if(millis()-last_wifi_check >= WIFI_CHECK_INTERVAL) {
      if(DEBUG_WIFI) Serial << millis() << " reconnecting to wifi" << endl;
      disconnectWifi();
      connectWifi();
      last_wifi_check = millis();
    }
  }

}


void connectWifi() {

  preferences.begin("app", true); // true means read only
  String ssid = preferences.getString("wifi_ssid"); 
  String password = preferences.getString("wifi_password");
  preferences.end();

  if (ssid == "" || password == ""){
    if(DEBUG_WIFI) Serial << "no values saved for wifi ssid or password" << endl;
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial << "connecting to wifi " << endl;
  connect_time = millis();
  WIFI_CONNECTING = true;
}


void disconnectWifi() {
  WiFi.disconnect();
  digitalWrite(LED_COMMS_PIN, LOW);
  if(DEBUG_WIFI) Serial << "disconnected from wifi " << WiFi.localIP() << endl;
}


void readPrefs() {

  preferences.begin("app", true); // true means read only

  Serial << "wifi_ssid: " << preferences.getString("wifi_ssid") << endl;
  Serial << "wifi_password: " << preferences.getString("wifi_password") << endl;

  preferences.end();

}

