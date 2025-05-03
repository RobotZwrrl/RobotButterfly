void initWifi() {

  pinMode(LED_COMMS_PIN, OUTPUT);

  Mutex_WIFI = xSemaphoreCreateMutex();  

  // core 0 has task watchdog enabled to protect wifi service etc
  // core 1 does not have watchdog enabled
  // can do this if wdt gives trouble: disableCore0WDT();
  xTaskCreatePinnedToCore(
                    Task_WIFI_code,     // task function
                    "Task_WIFI",        // name of task
                    STACK_WIFI,         // stack size of task
                    NULL,               // parameter of the task
                    PRIORITY_WIFI_MID,  // priority of the task (low number = low priority)
                    &Task_WIFI,         // task handle to keep track of created task
                    TASK_CORE_WIFI);    // pin task to core 0

}


void Task_WIFI_code(void * pvParameters) {
  while(1) {

    // update task mon
    wifiTaskMon.task_enter = millis();
    wifiTaskMon.task_priority = uxTaskPriorityGet(Task_WIFI);

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_WIFI, (TickType_t)10) == pdTRUE) {
      
      updateWifi();

      // give mutex after critical section
      xSemaphoreGive(Mutex_WIFI);
    }
    
    // update our task mon
    wifiTaskMon.task_exit = millis();

    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_WIFI );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void updateWifi() {

  if(WIFI_CONNECT_ON_STARTUP == true && WiFi.status() != WL_CONNECTED && WIFI_CONNECTING == false && connect_time == 0) {
    if(DEBUG_WIFI) Serial << "connect wifi on startup" << endl;
    connectWifi();
  }

  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_COMMS_PIN, HIGH);
    if(JUST_CONNECTED) {
      JUST_CONNECTED = false;
      if(DEBUG_WIFI) Serial << endl;
      if(DEBUG_WIFI) Serial << "connected to wifi " << WiFi.localIP() << endl;
    }

    return;
  }

  // waiting to connect
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
    if(millis()-last_wifi_check >= (WIFI_CONNECTION_TIMEOUT+WIFI_CHECK_INTERVAL)) {
      if(DEBUG_WIFI) Serial << millis() << " reconnecting to wifi" << endl;
      
      disconnectWifi();
      connectWifi();

      last_wifi_check = millis();
    }
  }

  return;
}


void connectWifi() {

  if(DEBUG_WIFI) Serial << "connectWifi()" << endl;

  preferences.begin("app", true); // true means read only
  String ssid = preferences.getString("wifi_ssid"); 
  String password = preferences.getString("wifi_password");
  preferences.end();

  if(ssid == "" || password == "") {
    if(DEBUG_WIFI) Serial << "no values saved for wifi ssid or password" << endl;
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  if(DEBUG_WIFI) Serial << "connecting to wifi " << endl;
  connect_time = millis();
  WIFI_CONNECTING = true;
  JUST_CONNECTED = true;

  return;
}


void disconnectWifi() {
  
  if(DEBUG_WIFI) Serial << "disconnectWifi()" << endl;

  WiFi.disconnect();
  digitalWrite(LED_COMMS_PIN, LOW);
  if(DEBUG_WIFI) Serial << "disconnected from wifi " << WiFi.localIP() << endl;
  
  return;
}


void readPrefs() {

  preferences.begin("app", true); // true means read only

  Serial << "wifi_ssid: " << preferences.getString("wifi_ssid") << endl;
  Serial << "wifi_password: " << preferences.getString("wifi_password") << endl;

  preferences.end();

}

