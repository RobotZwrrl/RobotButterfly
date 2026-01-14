#include "modules/DeviceMQTT/DeviceMQTT.h"

IOTMessageCallback onIOTMessageReceivedCallback = NULL;
IOTMessageCallback onIOTMessagePublishedCallback = NULL;
IOTCallback onIOTConnectedCallback = NULL;
IOTCallback onIOTDisconnectedCallback = NULL;

IOTMessageCallback RobotButterfly::onIOTMessageReceivedCallback_client = NULL;
IOTMessageCallback RobotButterfly::onIOTMessagePublishedCallback_client = NULL;
IOTCallback RobotButterfly::onIOTConnectedCallback_client = NULL;
IOTCallback RobotButterfly::onIOTDisconnectedCallback_client = NULL;

struct DeviceMQTT device_mqtt;
struct TelemetryMQTT telemetry_mqtt;
struct Telemetry10hzMQTT telemetry_10hz_mqtt;

// ------------------------------------
// ------------------------------------

void initMQTT() {

  device_mqtt.wifi_ssid = RobotButterfly::getPreference(SETTINGS_WIFI_SSID);
  device_mqtt.wifi_pass = RobotButterfly::getPreference(SETTINGS_WIFI_PASS);
  device_mqtt.mqtt_server = RobotButterfly::getPreference(SETTINGS_MQTT_SERVER);
  device_mqtt.mqtt_port = RobotButterfly::getPreference(SETTINGS_MQTT_PORT);
  device_mqtt.mqtt_user = RobotButterfly::getPreference(SETTINGS_MQTT_USER);
  device_mqtt.mqtt_pass = RobotButterfly::getPreference(SETTINGS_MQTT_PASS);
  device_mqtt.mqtt_id = RobotButterfly::getPreference(SETTINGS_MQTT_ID);
  device_mqtt.robot_name = RobotButterfly::getPreference(SETTINGS_ROBOT_NAME);
  device_mqtt.robot_namespace = RobotButterfly::getPreference(SETTINGS_NAMESPACE);
  device_mqtt.robot_set = RobotButterfly::getPreference(SETTINGS_SET);
  device_mqtt.robot_team = RobotButterfly::getPreference(SETTINGS_TEAM);

  done_connecting = false;
  done_disconnecting = true;
  mqtt_task_enabled = true;
  time_to_publish = false;
  time_to_10hz_publish = false;

  last_neoanim_mqtt_msg = 0;

  if(RTOS_ENABLED == true && Mutex_MQTT == NULL) {

    Mutex_MQTT = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_MQTT_code,     // task function
                      "Task_MQTT",        // name of task
                      STACK_MQTT,         // stack size of task
                      NULL,               // parameter of the task
                      PRIORITY_MQTT_LOW,  // priority of the task (low number = low priority)
                      &Task_MQTT,         // task handle to keep track of created task
                      TASK_CORE_MQTT);    // pin task to core

    return;

  }

  if(!RTOS_ENABLED) return;

  setMQTTTaskPriority(PRIORITY_MQTT_LOW); // calling again just incase we init again, this will reset the priority

}


void connectMQTT() {

  if(DEBUG_MQTT) Serial << "checking wifi..." << endl;

  if(WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(device_mqtt.mqtt_id.c_str());
    
    if(DEBUG_MQTT) Serial << "wifi info: " << device_mqtt.wifi_ssid.c_str() << ", " << device_mqtt.wifi_pass.c_str() << endl;
    WiFi.begin(device_mqtt.wifi_ssid.c_str(), device_mqtt.wifi_pass.c_str());
    
    if(DEBUG_MQTT) Serial << "wifi not connected yet" << endl;
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    return; // only proceed once connected to wifi
  } else {
    if(DEBUG_MQTT) Serial << "wifi connected!" << endl;
  }

  if(DEBUG_MQTT) Serial << "checking mqtt..." << endl;
    
  if(mqtt.connected() != true) {
    mqtt.disconnect();
    mqtt.begin(device_mqtt.mqtt_server.c_str(), device_mqtt.mqtt_port.toInt(), net);
    mqtt.onMessage(messageReceivedMQTT);
    mqtt.setWill( (device_mqtt.mqtt_id+"/status").c_str() , String("offline").c_str(), true, 0);
    
    if(DEBUG_MQTT) Serial << "mqtt info: " << device_mqtt.mqtt_id.c_str() << ", " << device_mqtt.mqtt_user.c_str() << ", " << device_mqtt.mqtt_pass.c_str() << endl;
    mqtt.connect(device_mqtt.mqtt_id.c_str(), device_mqtt.mqtt_user.c_str(), device_mqtt.mqtt_pass.c_str());
    
    if(DEBUG_MQTT) Serial << "mqtt not connected yet" << endl;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return; // only proceed once connected to mqtt
  } else {
    if(DEBUG_MQTT) Serial << "mqtt connected!" << endl;
  }

  if(mqtt.connected() == true && WiFi.status() == WL_CONNECTED) {
    if(DEBUG_MQTT) Serial << "all connected!" << endl;
    if(onIOTConnectedCallback) onIOTConnectedCallback();
    done_connecting = true;
    defaultSubscribeMQTT();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    defaultPublishMQTT();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    publishMQTT();
  }

}


bool isIotConnected() {
  if(mqtt.connected() == true && WiFi.status() == WL_CONNECTED) {
    return true;
  }
  return false;
}


void defaultSubscribeMQTT() {
  mqtt.subscribe( (device_mqtt.mqtt_id+"/message") );
}


void conductorSubscribeMQTT() {
  mqtt.subscribe( (device_mqtt.robot_namespace+"/control") );
  mqtt.subscribe( (device_mqtt.robot_namespace+"/"+device_mqtt.robot_set+"/control") );
  mqtt.subscribe( (device_mqtt.robot_namespace+"/"+device_mqtt.robot_set+"/"+device_mqtt.robot_team+"/control") );
}


void subscribeMQTT(String topic) {
  if(DEBUG_MQTT) Serial << "subscribing: " << topic << endl;
  mqtt.subscribe( topic );
}


void defaultPublishMQTT() {
  mqtt.publish( (device_mqtt.mqtt_id+"/namespace"), device_mqtt.robot_namespace);
  mqtt.publish( (device_mqtt.mqtt_id+"/set"), device_mqtt.robot_set);
  mqtt.publish( (device_mqtt.mqtt_id+"/team"), device_mqtt.robot_team);
}


// imu's timer counts and sets a flag, which then
// robotbutterfly checks, and then calls this. a
// flag is set for the main update mqtt loop to
// publish the data to avoid using a delay for periodicity
void publishMQTT() {
  time_to_publish = true;
}


void publish10hzMQTT() {
  time_to_10hz_publish = true;
}
// ------


void publishMQTTMessage(String topic, String payload) {
  bool b = mqtt.publish(device_mqtt.mqtt_id+"/"+topic, payload);
  if(onIOTMessagePublishedCallback != NULL && b == true) onIOTMessagePublishedCallback(topic, payload);
}


void telemetryPublish() {

  if(DEBUG_MQTT) Serial << millis() << " mqtt telemetry publish" << endl;
  mqtt.publish( (device_mqtt.mqtt_id+"/status"), "online");

  // TODO: all the other sensors

  mqtt.publish( (device_mqtt.mqtt_id+"/environment/temperature"), String(telemetry_mqtt.temperature));
  mqtt.publish( (device_mqtt.mqtt_id+"/environment/humidity"), String(telemetry_mqtt.humidity));
  mqtt.publish( (device_mqtt.mqtt_id+"/environment/light"), String(telemetry_mqtt.light));
  mqtt.publish( (device_mqtt.mqtt_id+"/environment/sound"), String(telemetry_mqtt.sound));
  mqtt.publish( (device_mqtt.mqtt_id+"/status/battery"), String(telemetry_mqtt.battery));

  /*
    telemetry data
    - neopixel pattern
    - neopixel primary and secondary colours
    - temp, humidity, light, sound values
  */

}


void telemetry10hzPublish() {

  if(DEBUG_MQTT) Serial << millis() << " mqtt telemetry 10hz publish" << endl;
  
  /*
  - proximity value will have to be published more frequently
  */

}


void updateMQTT() {

  if(done_disconnecting == false) {
    if(DEBUG_MQTT) Serial << "mqtt disconnecting" << endl;
    mqtt.disconnect();
    if(DEBUG_MQTT) Serial << "wifi disconnecting" << endl;
    WiFi.disconnect();
    if(onIOTDisconnectedCallback) onIOTDisconnectedCallback();
    done_disconnecting = true;
    mqtt_task_enabled = false;
    if(DEBUG_MQTT) Serial << "zzz mqtt task going to be suspended" << endl;
    vTaskSuspend(Task_MQTT);
    return;
  }

  if(!mqtt_task_enabled) return;

  if(done_connecting) {
    if(mqtt.connected() != true || WiFi.status() != WL_CONNECTED) {
      if(onIOTDisconnectedCallback) onIOTDisconnectedCallback();
    }
  }

  if(mqtt.connected() != true || WiFi.status() != WL_CONNECTED) {
    done_connecting = false;
  }

  if(!done_connecting) {
    connectMQTT();
  } else {
    mqtt.loop();

    // these flags are originally set by the imu timer,
    // which the main loop of robot butterfly looks
    // for, and then sets these flag in devicemqtt.
    // now in this loop, we look for that flag, and
    // then publish when ready.
    // oh yeah, and don't forget to reset the flag!

    if(time_to_publish) {
      telemetryPublish();
      time_to_publish = false;
    }

    if(time_to_10hz_publish) {
      telemetry10hzPublish();
      time_to_10hz_publish = false;
    }

  }

}


void messageReceivedMQTT(String &topic, String &payload) {
  if(DEBUG_MQTT) Serial << "incoming mqtt message: " << topic << ": " << payload << endl;
  if(onIOTMessageReceivedCallback) onIOTMessageReceivedCallback(topic, payload);
}



void Task_MQTT_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_MQTT, (TickType_t)10) == pdTRUE) {
      
      if(!mqtt_task_enabled) return;

      updateMQTT();

      // if(DEBUG_MQTT_RTOS == true && millis()-last_mqtt_rtos_print >= 1000) {
      //   Serial << "mqtt stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
      //   Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
      //   last_mqtt_rtos_print = millis();
      // }

      // give mutex after critical section
      xSemaphoreGive(Mutex_MQTT);
    }
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_MQTT != 0) {
      vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_MQTT );
    } else {
      vTaskDelay(1);
    }
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setMQTTTaskPriority(uint8_t p) {
  
  if(!RTOS_ENABLED) return;

  if(p == PRIORITY_MQTT_OFF || MQTT_MODE_ACTIVE == false) {
    if(DEBUG_MQTT_RTOS) Serial << "suspending mqtt task" << endl;
    done_disconnecting = false;
    // TODO: fix this
  } else {
    uint8_t prev_priority = uxTaskPriorityGet(Task_MQTT);
    vTaskPrioritySet(Task_MQTT, p);

    if(Task_MQTT != NULL) {
      eTaskState state = eTaskGetState(Task_MQTT);
      if(state == eSuspended) {
        if(DEBUG_MQTT_RTOS) Serial << "resuming mqtt task" << endl;
        vTaskResume(Task_MQTT);
      }
      if(DEBUG_MQTT_RTOS) Serial << "enabling mqtt task" << endl;
      mqtt_task_enabled = true;
    }

    if(DEBUG_MQTT_RTOS) Serial << "changed mqtt task priority - new: " << p << " prev: " << prev_priority << endl;
  }

}


String getMacAddress() {
  // Get WiFi MAC and make it client-safe
  uint64_t chipid = ESP.getEfuseMac();
  return String((uint32_t)(chipid >> 32), HEX) + String((uint32_t)chipid, HEX);
}


int splitTopic(const String &topic, String *parts) {

  int count = 0;
  int start = 0;

  for(uint8_t i=0; i<topic.length() && count < MAX_SUBTOPICS-1; i++) {

    if(topic.charAt(i) == '/') {
      parts[count] = topic.substring(start, i);
      count++;
      start = i+1;
    }

  }

  if(count < MAX_SUBTOPICS) {
    parts[count] = topic.substring(start);
    count++;
  }

  return count;
}



