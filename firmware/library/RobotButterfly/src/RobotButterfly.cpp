#include "RobotButterfly.h"

// -- mqtt --
volatile bool MQTT_MODE_ACTIVE = false;
volatile bool did_autoconnect = false;
volatile bool iot_publish_timer_flag = false;
volatile bool iot_publish_10hz_flag = false;
// --

// -- state machine --
volatile uint8_t MAX_STATE = 4;
volatile bool new_enter = false;
volatile bool new_update = false;
volatile bool enter_state = false;
volatile bool update_state = false;
volatile bool new_print = false;
// --

// -- servo calibration --
volatile bool SERVO_CAL_MODE = false;
volatile bool left_cal_dir = true;
volatile int left_cal_mode = 0;
volatile bool right_cal_dir = true;
volatile int right_cal_mode = 0;
volatile bool button_calib_changed = false;
volatile int servo_calib_pos_left = 0;
volatile int servo_calib_pos_right = 0;
// --

// -- startup --
volatile bool BATTERY_AA_MODE = true;
volatile long start_del = 0;
volatile bool hold_notif_action = false;
// --

// -- eeprom prefs --
volatile bool eeprom_mode = false;
volatile bool command_select = false;
volatile bool entering_value = false;
volatile char command_key = ' ';
Preferences preferences;
// --


hw_timer_t* RobotButterfly::timer_state_cfg = NULL;

uint8_t RobotButterfly::CURRENT_STATE = 0;
uint8_t RobotButterfly::PREV_STATE = 0;
RobotButterfly::State* RobotButterfly::all_states[NUM_STATES] = { NULL };
bool RobotButterfly::CHANGE_STATES_CONTROL = true;
uint8_t RobotButterfly::MAX_STATES_COUNT = 0;

RobotButterfly::State RobotButterfly::state1;
RobotButterfly::State RobotButterfly::state2;
RobotButterfly::State RobotButterfly::state3;
RobotButterfly::State RobotButterfly::state4;
RobotButterfly::State RobotButterfly::state5;
RobotButterfly::State RobotButterfly::state6;
RobotButterfly::State RobotButterfly::state7;
RobotButterfly::State RobotButterfly::state8;

void IRAM_ATTR Timer_State_ISR() {
  enter_state = false;
  update_state = true;
  new_print = true;
  new_update = true;
}


RobotButterfly::RobotButterfly() {
}


// init_servos refers to if initServos is called in this function
// this is useful in cases where checking for low power mode (eg, 
// on AA batts)
// state_machine refers to automatically using the buttons to
// increment / decrement the state. for simple sketches this might
// not be desired
void RobotButterfly::init(bool init_servos, bool state_machine_control) {
  
  WiFi.mode(WIFI_OFF);
  btStop();
  setCpuFrequencyMhz(80);
  MQTT_MODE_ACTIVE = false;

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  pinMode(LED_COMMS_PIN, OUTPUT);

  initSound();
  initNeopixels();
  initNeoAnimations();

  // -- buffer --
  // this playing multiple times on startup is an
  // indicator that the robot is being powered by
  // AA batteries because a few brownouts will
  // occur as the robot stabilises on startup when
  // moving its wings to the initial position.
  // having this sound helps for debugging and also
  // gives a cheeky characteristic (sounds like a 
  // robot giggling haha).
  playSound(SOUND_FLUTTER_JOY);

  setNeoAnim(&neo_animation_home, NEO_ANIM_FUNKY, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_SKY_BLUE);
  setNeoAnimSpeed(&neo_animation_home, 300);
  startNeoAnim(&neo_animation_home);

  start_del = millis();
  while(millis()-start_del < 300) {
    updateNeoAnimation();
    updateSound();
    delay(1);
  }
  // --

  initStateMachine();
  initButtons();
  if(init_servos) {
    // initialise the servos depending on if the AA
    // batteries are in use or not. if they are,
    // take some time to initialise each side
    batteryCheck();
  }
  initIMU();
  initSensors();
  initProximity();
  
  setStartupPriorities();
  setAnimations();

  CHANGE_STATES_CONTROL = state_machine_control;
  MAX_STATES_COUNT = 0;

  // -- mqtt callbacks --
  onIOTMessageReceivedCallback = iotMessageReceivedCallback;
  onIOTMessagePublishedCallback = iotMessagePublishedCallback;
  onIOTConnectedCallback = iotConnectedCallback;
  onIOTDisconnectedCallback = iotDisconnectedCallback;

  onIOTMessageReceivedCallback_client = NULL;
  onIOTMessagePublishedCallback_client = NULL;
  onIOTConnectedCallback_client = NULL;
  onIOTDisconnectedCallback_client = NULL;
  // --

  // -- button callbacks --
  onHoldNotificationCallback = buttonHoldNotificationCallback;
  onHoldReleasedCallback = buttonHoldReleasedCallback;
  onClickCallback = buttonClickCallback;
  onReleaseCallback = buttonReleaseCallback;

  onHoldNotificationCallback_client = NULL;
  onHoldReleasedCallback_client = NULL;
  onClickCallback_client = NULL;
  onReleaseCallback_client = NULL;
  // --

  // -- sound callbacks --
  onSoundDoneCallback = soundDoneCallback;

  onSoundDoneCallback_client = NULL;
  // --

  // -- imu callbacks --
  onStateChangeCallback = imuStateChangeCallback;
  onOrientationChangeCallback = imuOrientationChangeCallback;
  onPoseChangeCallback = imuPoseChangeCallback;
  onEventDetectedCallback = imuEventDetectedCallback;

  onStateChangeCallback_client = NULL;
  onOrientationChangeCallback_client = NULL;
  onPoseChangeCallback_client = NULL;
  onEventDetectedCallback_client = NULL;
  // --

  // -- neoanim callbacks --
  onNeoAnimStartCallback = neoAnimStartCallback;
  onNeoAnimDoneCallback = neoAnimDoneCallback;
  onNeoAnimLoopCallback = neoAnimLoopCallback;

  onNeoAnimStartCallback_client = NULL;
  onNeoAnimDoneCallback_client = NULL;
  onNeoAnimLoopCallback_client = NULL;
  // --

  // -- servoanim callbacks --
  onServoAnimDoneCallback = servoAnimDoneCallback;
  onServoAnimLoopCallback = servoAnimLoopCallback;
    
  onServoAnimDoneCallback_client = NULL;
  onServoAnimLoopCallback_client = NULL;
  // --

  // -- sensor callbacks --
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorChangeCallback = sensorHumidityChangeCallback;
  all_sensors[SENSOR_ID_HUMIDITY]->onSensorAmbientChangeCallback = sensorHumidityAmbientChangeCallback;

  all_sensors[SENSOR_ID_LIGHT]->onSensorChangeCallback = sensorLightChangeCallback;
  all_sensors[SENSOR_ID_LIGHT]->onSensorAmbientChangeCallback = sensorLightAmbientChangeCallback;

  all_sensors[SENSOR_ID_SOUND]->onSensorChangeCallback = sensorSoundChangeCallback;
  all_sensors[SENSOR_ID_SOUND]->onSensorAmbientChangeCallback = sensorSoundAmbientChangeCallback;

  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorChangeCallback = sensorTemperatureChangeCallback;
  all_sensors[SENSOR_ID_TEMPERATURE]->onSensorAmbientChangeCallback = sensorTemperatureAmbientChangeCallback;
  
  onSensorLightChangeCallback_client = NULL;
  onSensorLightAmbientChangeCallback_client = NULL;
  onSensorSoundChangeCallback_client = NULL;
  onSensorSoundAmbientChangeCallback_client = NULL;
  onSensorTemperatureChangeCallback_client = NULL;
  onSensorTemperatureAmbientChangeCallback_client = NULL;
  onSensorHumidityChangeCallback_client = NULL;
  onSensorHumidityAmbientChangeCallback_client = NULL;
  // --

  // -- proximity callbacks --
  ultrasonic.onProximityTriggerCallback = proximityTriggerCallback;
  
  onProximityTriggerCallback_client = NULL;
  // --

}


// only updates the state machine
// - good for when rtos mode is enabled
void RobotButterfly::update() {

  // servo calibration mode 'overwrites' all the states
  if(SERVO_CAL_MODE) {

    if(button_calib_changed) {

      if(left_cal_mode == 0) { // wing up
        servo_calib_pos_left = SERVO_ANIM_POSITION_UP;
      } else if(left_cal_mode == 1) { // wing home
        servo_calib_pos_left = SERVO_ANIM_POSITION_HOME;
      } else if(left_cal_mode == 2) { // wing down
        servo_calib_pos_left = SERVO_ANIM_POSITION_DOWN;
      }

      if(right_cal_mode == 0) { // wing up
        servo_calib_pos_right = SERVO_ANIM_POSITION_UP;
      } else if(right_cal_mode == 1) { // wing home
        servo_calib_pos_right = SERVO_ANIM_POSITION_HOME;
      } else if(right_cal_mode == 2) { // wing down
        servo_calib_pos_right = SERVO_ANIM_POSITION_DOWN;
      }

      setServoAnim(&servo_animation_alert, SERVO_ANIM_POSITION, SERVO_ANIM_ALERT);
      setServoAnimRepeats(&servo_animation_alert, -99);
      setServoAnimPositionLeft(&servo_animation_alert, servo_calib_pos_left);
      setServoAnimPositionRight(&servo_animation_alert, servo_calib_pos_right);
      startServoAnim(&servo_animation_alert);

      setNeoAnim(&neo_animation_alert, NEO_ANIM_UNO, NEO_ANIM_ALERT);
      setNeoAnimColours(&neo_animation_alert, NEO_GREEN, NEO_GREEN);
      setNeoAnimSpeed(&neo_animation_alert, 1000);
      setNeoAnimUno(&neo_animation_alert, left_cal_mode); // top-most leds near the back
      setNeoAnimDuo(&neo_animation_alert, 5+right_cal_mode); // bottom-most leds near the front
      startNeoAnim(&neo_animation_alert);

      button_calib_changed = false;
    }

  } else {

    updateStateMachine();

    if(millis() >= 5000 && did_autoconnect == false) {
      if( getPreference(SETTINGS_IOT_AUTOCONNECT) == "1" ) {
        enableMQTT();
        did_autoconnect = true;
      }
    }

    // --- iot updates ---
    if(MQTT_MODE_ACTIVE) {

      // set by imu's timer
      if(iot_publish_timer_flag) {

        telemetry_mqtt.temperature = getSensor_Temperature(all_sensors[SENSOR_ID_TEMPERATURE]);
        telemetry_mqtt.humidity = getSensor_Humidity(all_sensors[SENSOR_ID_HUMIDITY]);
        telemetry_mqtt.sound = getSensor_Sound(all_sensors[SENSOR_ID_SOUND]);
        telemetry_mqtt.light = getSensor_Light(all_sensors[SENSOR_ID_LIGHT]);
        telemetry_mqtt.battery = getSensor_Battery(all_sensors[SENSOR_ID_BATTERY]);
        //telemetry_mqtt.zoop = 10;
        
        publishMQTT();
        iot_publish_timer_flag = false;
      }

      // set by imu's timer
      if(iot_publish_10hz_flag) {
        publish10hzMQTT();
        iot_publish_10hz_flag = false;
      }

    }
    // ------------------

  }
  
}


void RobotButterfly::parseNeoanimationMQTT(const String &json) {

  JsonDocument doc;
  deserializeJson(doc, json);

  neo_animation_mqtt.id = doc["id"];
  neo_animation_mqtt.active = doc["active"];
  neo_animation_mqtt.type = doc["type"];

  neo_animation_mqtt.colour_primary = doc["colour_primary"];
  neo_animation_mqtt.colour_secondary = doc["colour_secondary"];

  neo_animation_mqtt.num_frames = doc["num_frames"];
  neo_animation_mqtt.frame_delay = doc["frame_delay"];
  neo_animation_mqtt.frame_index = doc["frame_index"];
  neo_animation_mqtt.last_frame = doc["last_frame"];
  
  neo_animation_mqtt.num_repeats = doc["num_repeats"];
  neo_animation_mqtt.repeat_count = doc["repeat_count"];
  neo_animation_mqtt.repeat_delay = doc["repeat_delay"];
  neo_animation_mqtt.last_repeat = doc["last_repeat"];

  neo_animation_mqtt.duration = doc["duration"];
  neo_animation_mqtt.start_time = doc["start_time"];

  neo_animation_mqtt.dir = doc["dir"];
  neo_animation_mqtt.helper1 = doc["helper1"];
  neo_animation_mqtt.helper2 = doc["helper2"];
  neo_animation_mqtt.helper3 = doc["helper3"];

}


void RobotButterfly::startNeoanimationMQTT(int anim_type) {

  // TODO: tidy this up

  if(anim_type == NEO_ANIM_HOME) {
    setNeoAnim(&neo_animation_home, neo_animation_mqtt.id, neo_animation_mqtt.type);
    
    neo_animation_home.id = neo_animation_mqtt.id;
    neo_animation_home.active = neo_animation_mqtt.active;
    neo_animation_home.type = neo_animation_mqtt.type;

    neo_animation_home.colour_primary = neo_animation_mqtt.colour_primary;
    neo_animation_home.colour_secondary = neo_animation_mqtt.colour_secondary;

    neo_animation_home.num_frames = neo_animation_mqtt.num_frames;
    neo_animation_home.frame_delay = neo_animation_mqtt.frame_delay;
    neo_animation_home.frame_index = neo_animation_mqtt.frame_index;
    neo_animation_home.last_frame = neo_animation_mqtt.last_frame;
    
    neo_animation_home.num_repeats = neo_animation_mqtt.num_repeats;
    neo_animation_home.repeat_count = neo_animation_mqtt.repeat_count;
    neo_animation_home.repeat_delay = neo_animation_mqtt.repeat_delay;
    neo_animation_home.last_repeat = neo_animation_mqtt.last_repeat;

    neo_animation_home.duration = neo_animation_mqtt.duration;
    neo_animation_home.start_time = neo_animation_mqtt.start_time;

    neo_animation_home.dir = neo_animation_mqtt.dir;
    neo_animation_home.helper1 = neo_animation_mqtt.helper1;
    neo_animation_home.helper2 = neo_animation_mqtt.helper2;
    neo_animation_home.helper3 = neo_animation_mqtt.helper3;

    startNeoAnim(&neo_animation_home);
  }

  if(anim_type == NEO_ANIM_ALERT) {
    setNeoAnim(&neo_animation_alert, neo_animation_mqtt.id, neo_animation_mqtt.type);
    
    neo_animation_alert.id = neo_animation_mqtt.id;
    neo_animation_alert.active = neo_animation_mqtt.active;
    neo_animation_alert.type = neo_animation_mqtt.type;

    neo_animation_alert.colour_primary = neo_animation_mqtt.colour_primary;
    neo_animation_alert.colour_secondary = neo_animation_mqtt.colour_secondary;

    neo_animation_alert.num_frames = neo_animation_mqtt.num_frames;
    neo_animation_alert.frame_delay = neo_animation_mqtt.frame_delay;
    neo_animation_alert.frame_index = neo_animation_mqtt.frame_index;
    neo_animation_alert.last_frame = neo_animation_mqtt.last_frame;
    
    neo_animation_alert.num_repeats = neo_animation_mqtt.num_repeats;
    neo_animation_alert.repeat_count = neo_animation_mqtt.repeat_count;
    neo_animation_alert.repeat_delay = neo_animation_mqtt.repeat_delay;
    neo_animation_alert.last_repeat = neo_animation_mqtt.last_repeat;

    neo_animation_alert.duration = neo_animation_mqtt.duration;
    neo_animation_alert.start_time = neo_animation_mqtt.start_time;

    neo_animation_alert.dir = neo_animation_mqtt.dir;
    neo_animation_alert.helper1 = neo_animation_mqtt.helper1;
    neo_animation_alert.helper2 = neo_animation_mqtt.helper2;
    neo_animation_alert.helper3 = neo_animation_mqtt.helper3;

    startNeoAnim(&neo_animation_alert);
  }

}


void RobotButterfly::stopNeoanimationMQTT(int anim_type) {
  
  if(anim_type == NEO_ANIM_HOME) {
    stopNeoAnim(&neo_animation_home);
  }

  if(anim_type == NEO_ANIM_ALERT) {
    stopNeoAnim(&neo_animation_alert);
  }

}



// parameters use an enum (UpdateOptions) to choose 
// what updates with more granularity
void RobotButterfly::update(uint8_t update_statemachine, 
                            uint8_t update_buttons, 
                            uint8_t update_sound, 
                            uint8_t update_imu, 
                            uint8_t update_neoanim, 
                            uint8_t update_servoanim, 
                            uint8_t update_sensors, 
                            uint8_t update_proximity,
                            uint8_t update_mqtt) {

  if(update_statemachine == UPDATE_STATEMACHINE_ON) updateStateMachine();
  if(update_buttons == UPDATE_BUTTONS_ON) updateButtons();
  if(update_sound == UPDATE_SOUND_ON) updateSound();
  if(update_imu == UPDATE_IMU_ON) updateIMU();
  if(update_neoanim == UPDATE_NEOANIM_ON) updateNeoAnimation();
  if(update_servoanim == UPDATE_SERVOANIM_ON) updateServoAnimation();
  if(update_sensors == UPDATE_SENSORS_ON) updateSensors();
  if(update_proximity == UPDATE_PROXIMITY_ON) updateProximity();
  if(update_mqtt == UPDATE_MQTT_ON && MQTT_MODE_ACTIVE == true) updateMQTT();
  
}


void RobotButterfly::setAnimations() {

  // servo animation home
  setServoAnim(&servo_animation_home, SERVO_ANIM_POSITION, SERVO_ANIM_HOME);
  setServoAnimPositionLeft(&servo_animation_home, SERVO_ANIM_POSITION_UP);
  setServoAnimPositionRight(&servo_animation_home, SERVO_ANIM_POSITION_UP);
  startServoAnim(&servo_animation_home);

  // servo animation alert
  setServoAnim(&servo_animation_alert, SERVO_ANIM_NONE, SERVO_ANIM_ALERT);
  startServoAnim(&servo_animation_alert);

  // neo animation home
  setNeoAnim(&neo_animation_home, NEO_ANIM_SQUIGGLE, NEO_ANIM_HOME);
  setNeoAnimColours(&neo_animation_home, NEO_GREEN, NEO_PURPLE);
  setNeoAnimDuration(&neo_animation_home, 500);
  startNeoAnim(&neo_animation_home);

  // neo animation alert
  setNeoAnim(&neo_animation_alert, NEO_ANIM_NONE, NEO_ANIM_ALERT);
  startNeoAnim(&neo_animation_alert);

}


// ----------------------------------
// ------------- mqtt ---------------
// ----------------------------------

void RobotButterfly::enableMQTT() {
  // inside of Timer_10Hz_imu_ISR,
  // iot will be published when:
  // iot_timer_counter >= IOT_TIMER_COUNTER_MAX-1
  MQTT_MODE_ACTIVE = true;
  setMQTTTaskPriority(PRIORITY_MQTT_LOW);
  setIMUTaskPriority(PRIORITY_IMU_LOW); // mqtt task needs the imu timer to set a flag
  initMQTT();
}

void RobotButterfly::disableMQTT() {
  MQTT_MODE_ACTIVE = false; 
  setMQTTTaskPriority(PRIORITY_MQTT_OFF);
}

void RobotButterfly::sendMQTTMessage(String topic, String payload) {
  publishMQTTMessage(topic, payload);
}

void RobotButterfly::conductNamespace(String action) {
  publishMQTTMessage( (device_mqtt.robot_namespace+"/control"), action);
}

void RobotButterfly::conductSet(String action) {
  publishMQTTMessage( (device_mqtt.robot_namespace+"/"+device_mqtt.robot_set+"/control"), action);
}

void RobotButterfly::conductTeam(String action) {
  publishMQTTMessage( (device_mqtt.robot_namespace+"/"+device_mqtt.robot_set+"/"+device_mqtt.robot_team+"/control"), action);
}

void RobotButterfly::conductorSubscribe() {
  conductorSubscribeMQTT();
}

void RobotButterfly::mqttSubscribe(String topic) {
  subscribeMQTT(topic);
}

void RobotButterfly::mqttPublish(String topic, String payload) {
  // publish already includes device_mqtt.mqtt_id+"/"
  publishMQTTMessage(topic, payload);
}

void RobotButterfly::mqttPublishNeoanimation(struct NeoAnimation *a) {

  JsonDocument doc;

  doc["id"] = a->id;
  doc["active"] = a->active;
  doc["type"] = a->type;

  doc["colour_primary"] = a->colour_primary;
  doc["colour_secondary"] = a->colour_secondary;

  doc["num_frames"] = a->num_frames;
  doc["frame_delay"] = a->frame_delay;
  doc["frame_index"] = a->frame_index;
  doc["last_frame"] = a->last_frame;

  doc["num_repeats"] = a->num_repeats;
  doc["repeat_count"] = a->repeat_count;
  doc["repeat_delay"] = a->repeat_delay;
  doc["last_repeat"] = a->last_repeat;

  doc["duration"] = a->duration;
  doc["start_time"] = a->start_time;

  doc["dir"] = a->dir;
  doc["helper1"] = a->helper1;
  doc["helper2"] = a->helper2;
  doc["helper3"] = a->helper3;

  String output;
  serializeJson(doc, output);

  if(a->type == NEO_ANIM_HOME) {
    publishMQTTMessage("neoanimation_home", output);
  } else if(a->type == NEO_ANIM_ALERT) {
    publishMQTTMessage("neoanimation_alert", output);
  }

}


void RobotButterfly::mqttParseNeoanimation(struct NeoAnimation *a, String payload) {

  JsonDocument doc;
  deserializeJson(doc, payload);

  a->id = doc["id"];
  a->active = doc["active"];
  a->type = doc["type"];

  a->colour_primary = doc["colour_primary"];
  a->colour_secondary = doc["colour_secondary"];

  a->num_frames = doc["num_frames"];
  a->frame_delay = doc["frame_delay"];
  a->frame_index = doc["frame_index"];
  a->last_frame = doc["last_frame"];

  a->num_repeats = doc["num_repeats"];
  a->repeat_count = doc["repeat_count"];
  a->repeat_delay = doc["repeat_delay"];
  a->last_repeat = doc["last_repeat"];

  a->duration = doc["duration"];
  a->start_time = doc["start_time"];

  a->dir = doc["dir"];
  a->helper1 = doc["helper1"];
  a->helper2 = doc["helper2"];
  a->helper3 = doc["helper3"];
}



// ----------------------------------
// ----------------------------------


// ----------------------------------
// ------------ settings ------------
// ----------------------------------

bool RobotButterfly::processConsole(String str) {

  if(str == "+++") {
    eeprom_mode = !eeprom_mode;
    manageSettings(str);
    return false;
  }

  if(eeprom_mode) {
    manageSettings(str);
    return false;
  }

  return true;
}


void RobotButterfly::manageSettings(String str) {

  if(eeprom_mode == true) {

    if(command_select == false && entering_value == false) {
      Serial << "::::: eeprom mode entered :::::" << endl;
      displaySettingsMenu();
      command_select = true;
      entering_value = false;
    }
    
    eepromMachine(str);

  } else if(str == "+++" && eeprom_mode == false) {
    Serial << "::::: eeprom mode exited :::::" << endl;
    command_select = false;
    entering_value = false;
  }

}


void RobotButterfly::eepromMachine(String str) {

  bool good_key = false;

  if(command_select == true && entering_value == false) {

    command_key = str[0];
    String mem = "";
    preferences.begin("app", true);

    switch(command_key) {
      case 'a': {
        mem = preferences.getString(SETTINGS_WIFI_SSID);
        Serial << "[a] wifi ssid (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'b': {
        mem = preferences.getString(SETTINGS_WIFI_PASS);
        Serial << "[b] wifi pass (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'c': {
        mem = preferences.getString(SETTINGS_MQTT_SERVER);
        Serial << "[c] mqtt server (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'd': {
        mem = preferences.getString(SETTINGS_MQTT_PORT);
        Serial << "[d] mqtt port (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'e': {
        mem = preferences.getString(SETTINGS_MQTT_USER);
        Serial << "[e] mqtt user (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'f': {
        mem = preferences.getString(SETTINGS_MQTT_PASS);
        Serial << "[f] mqtt pass (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'g': {
        mem = preferences.getString(SETTINGS_MQTT_ID);
        Serial << "[g] mqtt id (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'i': {
        mem = preferences.getString(SETTINGS_ROBOT_NAME);
        Serial << "[i] robot name (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'j': {
        mem = preferences.getString(SETTINGS_NAMESPACE);
        Serial << "[j] robot namespace (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'k': {
        mem = preferences.getString(SETTINGS_SET);
        Serial << "[k] robot set (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case 'l': {
        mem = preferences.getString(SETTINGS_TEAM);
        Serial << "[l] robot team (" << mem << ")" << endl;
        good_key = true;
      }
      break;
      case ';': {
        mem = preferences.getString(SETTINGS_IOT_AUTOCONNECT);
        Serial << "[;] iot autoconnect (enter a 1 or 0) (" << mem << ")" << endl;
        good_key = true;
      }
      break;
    }

    preferences.end();

    if(good_key) {
      command_select = false;
      entering_value = true;
    }

  } else if(command_select == false && entering_value == true) {

    preferences.begin("app", false);

    switch(command_key) {
      case 'a': {
        preferences.putString(SETTINGS_WIFI_SSID, str);
        Serial << "set the wifi ssid to: " << str << endl;
        good_key = true;
      }
      break;
      case 'b': {
        preferences.putString(SETTINGS_WIFI_PASS, str);
        Serial << "set the wifi pass to: " << str << endl;
        good_key = true;
      }
      break;
      case 'c': {
        preferences.putString(SETTINGS_MQTT_SERVER, str);
        Serial << "set the mqtt server to: " << str << endl;
        good_key = true;
      }
      break;
      case 'd': {
        preferences.putString(SETTINGS_MQTT_PORT, str);
        Serial << "set the mqtt port to: " << str << endl;
        good_key = true;
      }
      break;
      case 'e': {
        preferences.putString(SETTINGS_MQTT_USER, str);
        Serial << "set the mqtt user to: " << str << endl;
        good_key = true;
      }
      break;
      case 'f': {
        preferences.putString(SETTINGS_MQTT_PASS, str);
        Serial << "set the mqtt pass to: " << str << endl;
        good_key = true;
      }
      break;
      case 'g': {
        preferences.putString(SETTINGS_MQTT_ID, str);
        Serial << "set the mqtt id to: " << str << endl;
        good_key = true;
      }
      break;
      case 'i': {
        preferences.putString(SETTINGS_ROBOT_NAME, str);
        Serial << "set the robot name to: " << str << endl;
        good_key = true;
      }
      break;
      case 'j': {
        preferences.putString(SETTINGS_NAMESPACE, str);
        Serial << "set the robot namespace to: " << str << endl;
        good_key = true;
      }
      break;
      case 'k': {
        preferences.putString(SETTINGS_SET, str);
        Serial << "set the robot set to: " << str << endl;
        good_key = true;
      }
      break;
      case 'l': {
        preferences.putString(SETTINGS_TEAM, str);
        Serial << "set the robot team to: " << str << endl;
        good_key = true;
      }
      break;
      case ';': {
        preferences.putString(SETTINGS_IOT_AUTOCONNECT, str);
        Serial << "set the iot autoconnect to: " << str << endl;
        good_key = true;
      }
      break;
    }

    preferences.end();

    if(good_key) {
      Serial << "::::: memory updated :::::" << endl;
      command_select = true;
      entering_value = false;
      displaySettingsMenu();
    }

  }

}


void RobotButterfly::displaySettingsMenu() {
  Serial << endl;
  Serial << "press the letter and hit enter. afterwards you will be prompted to enter the value" << endl;
  Serial << "[a] wifi ssid" << endl;
  Serial << "[b] wifi pass" << endl;
  Serial << "[c] mqtt server" << endl;
  Serial << "[d] mqtt port" << endl;
  Serial << "[e] mqtt user" << endl;
  Serial << "[f] mqtt pass" << endl;
  Serial << "[g] mqtt id" << endl;
  Serial << "[i] robot name" << endl;
  Serial << "[j] robot namespace" << endl;
  Serial << "[k] robot set" << endl;
  Serial << "[l] robot team" << endl;
  Serial << "[;] iot autoconnect" << endl;
  Serial << "[+++] exit" << endl;
  Serial << endl;
}


String RobotButterfly::getPreference(String key) {
  preferences.begin("app", true);
  String s = preferences.getString(key.c_str());
  preferences.end();
  return s;
}

// ----------------------------------
// ----------------------------------


// ----------------------------------
// --------- state machine ----------
// ----------------------------------

void RobotButterfly::initStateMachine() {

  enter_state = false;
  update_state = false;
  new_print = false;
  new_enter = false;
  new_update = false;

  // -- init state machine --
  all_states[STATE1] = &state1;
  all_states[STATE2] = &state2;
  all_states[STATE3] = &state3;
  all_states[STATE4] = &state4;
  all_states[STATE5] = &state5;
  all_states[STATE6] = &state6;
  all_states[STATE7] = &state7;
  all_states[STATE8] = &state8;
  // --

}


void RobotButterfly::updateStateMachine() {

  if(SERVO_CAL_MODE) return;

  State *s = all_states[CURRENT_STATE];
  if(s == NULL) return;

  // state entrance
  if(enter_state) {
    
    if(s->enabled) {
      if(s->setup_fn) s->setup_fn();
    }

  }
  
  // state loop
  if(update_state) {

    s->t_enter = millis();
    s->t_delta = s->t_enter - s->t_transition;
    //if(DEBUG_STATEMACHINE) Serial << "delta: " << s.t_delta << " enter (" << s.t_enter << ") - transition (" << s.t_transition << ")" << endl;

    if(s->enabled) {
      if(s->loop_fn) s->loop_fn();
    }
    
  }

}


void RobotButterfly::transitionState() {

  if(DEBUG_STATEMACHINE) Serial << "transitioning state" << endl;

  if(all_states[CURRENT_STATE] == NULL) return;
  if(all_states[PREV_STATE] == NULL) return;

  all_states[PREV_STATE]->enabled = false;
  all_states[CURRENT_STATE]->enabled = true;

  State *s = all_states[CURRENT_STATE];

  // start the transition state process 
  // (eg, to show the lights representing which state number)
  s->t_transition = millis();
  update_state = false;

  // clean up old timer
  if (timer_state_cfg) {
    timerAlarmDisable(timer_state_cfg);
    timerDetachInterrupt(timer_state_cfg);
    timerEnd(timer_state_cfg);
    timer_state_cfg = NULL;
  }

  // timer transition - timer 0
  timer_state_cfg = timerBegin(0, 8000, true);
  timerAttachInterrupt(timer_state_cfg, &Timer_State_ISR, true);
  // params: timer, tick count, auto-reload (false to run once)
  timerAlarmWrite(timer_state_cfg, TRANSITION_FRAME_TIME, false);
  timerAlarmEnable(timer_state_cfg);
  enter_state = true;
  new_enter = true;

  s->last_state_change = millis();
  if(DEBUG_STATEMACHINE) Serial << "entering state index: " << CURRENT_STATE << " from index: " << PREV_STATE << endl;
}


void RobotButterfly::setNumStates(uint8_t n) {
  if(n > 8) n = 8;
  MAX_STATE = n; // apologies for the confusing naming
}


void RobotButterfly::addState(uint8_t id, StateSetup setup_fn, StateLoop loop_fn) {
  if(id > NUM_STATES-1) return;
  all_states[id]->id = id;
  all_states[id]->setup_fn = setup_fn;
  all_states[id]->loop_fn = loop_fn;
  all_states[id]->enabled = true;
  MAX_STATES_COUNT++;
}


void RobotButterfly::changeState(uint8_t id) {
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE = id;
  if(CURRENT_STATE >= MAX_STATES_COUNT) CURRENT_STATE = MAX_STATES_COUNT-1;
  transitionState();
}


void RobotButterfly::incrementState() {
  if(SERVO_CAL_MODE) return;
  PREV_STATE = CURRENT_STATE;
  CURRENT_STATE++;
  if(CURRENT_STATE >= MAX_STATES_COUNT) {
    CURRENT_STATE = 0; // loop around
  }
  transitionState();
}


void RobotButterfly::decrementState() {
  if(SERVO_CAL_MODE) return;
  PREV_STATE = CURRENT_STATE;
  if(CURRENT_STATE == 0) {
    CURRENT_STATE = MAX_STATES_COUNT-1; // loop around
  } else {
    CURRENT_STATE--;
  }
  transitionState();
}


void RobotButterfly::printStateHeartbeat(uint8_t id) {

  State *s = all_states[id];
  if(s == NULL) return;

  if(millis()-s->last_state_print >= STATE_LOOP_PRINT || s->new_print == true) {
    Serial << "STATE " << id << " loop" << endl;
    s->new_print = false;
    s->last_state_print = millis();
  }

}


void RobotButterfly::batteryCheck() {

  int battery_val = analogRead(BATT_SIG_PIN);
  if(battery_val > 0) {
    Serial << battery_val << " BATTERY_AA_MODE enabled" << endl;
    BATTERY_AA_MODE = true;
  } else {
    Serial << battery_val << " BATTERY_AA_MODE disabled" << endl;
    BATTERY_AA_MODE = false;
  }

  if(BATTERY_AA_MODE == true) {

    initServos(SERVO_MODE_INIT_LEFT);
    
    // battery catch up
    // update can be called here from setup() as the
    // rtos scheduler has not started yet - interesting!
    setNeoAnimColours(&neo_animation_home, NEO_ORANGE, NEO_SKY_BLUE);
    startNeoAnim(&neo_animation_home);
    start_del = millis();
    while(millis()-start_del < 1200) {
      updateNeoAnimation();
      delay(1);
    }

    initServos(SERVO_MODE_INIT_RIGHT);

    // battery catch up
    // update can be called here from setup() as the
    // rtos scheduler has not started yet - interesting!
    setNeoAnimColours(&neo_animation_home, NEO_CANARY_YELLOW, NEO_GREEN);
    startNeoAnim(&neo_animation_home);
    start_del = millis();
    while(millis()-start_del < 1200) {
      updateNeoAnimation();
      delay(1);
    }

    initialised_servos = true; // remember to set the flag when initialising motors separately
    initServoAnimations();

  } else {

    initServos(SERVO_MODE_INIT_BOTH);
    initServoAnimations();

  }

}


// turn some of unused tasks to idle while
// the robot is starting up
void RobotButterfly::setStartupPriorities() {
  setButtonsTaskPriority(PRIORITY_BUTTONS_MID);
  setIMUTaskPriority(tskIDLE_PRIORITY);
  setNeoAnimationTaskPriority(PRIORITY_NEOANIM_MID);
  setProximityTaskPriority(tskIDLE_PRIORITY);
  setSensorsTaskPriority(tskIDLE_PRIORITY);
  setServoAnimationTaskPriority(PRIORITY_SERVOANIM_HIGH);
  setSoundTaskPriority(PRIORITY_SOUND_MID);
}

// ----------------------------------
// ----------------------------------


