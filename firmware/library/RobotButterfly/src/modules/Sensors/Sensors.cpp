#include "modules/Sensors/Sensors.h"

DHT dht(DHT11_PIN, DHT11);

Sensor sensor_temperature;
Sensor sensor_humidity;
Sensor sensor_light;
Sensor sensor_sound;
Sensor sensor_battery;

Sensor* all_sensors[NUM_SENSORS] = {
  &sensor_temperature,
  &sensor_humidity,
  &sensor_light,
  &sensor_sound,
  &sensor_battery
};


// ------------- sensor isr --------------
void IRAM_ATTR Timer_10Hz_ISR() { // every 0.1 seconds

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;

    // update raw every 0.1 second
    if(s->iteration_raw >= s->reload_raw-1) {
      s->update_raw = true; 
      s->iteration_raw = 0;
    } else {
      if(s->update_raw == false) {
        s->iteration_raw++;
      }
    }

    // update val every 1 second
    if(s->iteration_val >= s->reload_val-1) {
      s->update_val = true;
      s->iteration_val = 0;
    } else {
      if(s->update_val == false) {
        s->iteration_val++;
      }
    }

    // update ambient every 60 seconds
    if(s->iteration_ambient >= s->reload_ambient-1) {
      s->update_ambient = true;
      s->iteration_ambient = 0;
    } else {
      if(s->update_ambient == false) {
        s->iteration_ambient++;
      }
    }

  }

}
// ------------------------------------


void processSensors(struct Sensor *s) {

  bool raw_update;
  uint8_t raw_iteration;
  uint8_t raw_reload;
  bool val_update;
  uint8_t val_iteration;
  uint8_t val_reload;
  bool ambient_update;
  uint16_t ambient_iteration;
  uint16_t ambient_reload;

  uint16_t raw_temporary;
  bool skip_raw = false;

  // make it atomic by copying to local variables
  noInterrupts();
    raw_update = s->update_raw;
    raw_iteration = s->iteration_raw;
    raw_reload = s->reload_raw;
    val_update = s->update_val;
    val_iteration = s->iteration_val;
    val_reload = s->reload_val;
    ambient_update = s->update_ambient;
    ambient_iteration = s->iteration_ambient;
    ambient_reload = s->reload_ambient;
  interrupts();

  // acquire the raw sensor reading,
  // add raw to the moving average
  if(raw_update) {
    noInterrupts();
      s->update_raw = false;
    interrupts();

    s->raw_prev = s->raw;
    
    // some extra logic for turn-taking with the dht11
    if(s->id != SENSOR_ID_TEMPERATURE && s->id != SENSOR_ID_HUMIDITY) {
      
      raw_temporary = s->getRawData(s);

    } else if(s->id == SENSOR_ID_TEMPERATURE 
              && dht_toggle == true 
              && dht_processed == false
              && millis()-last_dht_processed >= 2000) {
      raw_temporary = s->getRawData(s);
      dht_toggle = !dht_toggle;
      dht_processed = true;  // don't process another dht this loop
      //sensor_temperature.iteration_raw = 0;
      sensor_humidity.iteration_raw = 0;
      last_dht_processed = millis();
    } else if(s->id == SENSOR_ID_HUMIDITY
              && dht_toggle == false
              && dht_processed == false
              && millis()-last_dht_processed >= 2000) {
      raw_temporary = s->getRawData(s);
      dht_toggle = !dht_toggle;
      dht_processed = true;  // don't process another dht this loop
      sensor_temperature.iteration_raw = 0;
      //sensor_humidity.iteration_raw = 0;
      last_dht_processed = millis();
    }

    // check for a bad value
    if(s->id == SENSOR_ID_TEMPERATURE || s->id == SENSOR_ID_HUMIDITY) {
      if(raw_temporary == 999 || raw_temporary == 0) { // bad val
        skip_raw = true;
      }
    }
    
    if(skip_raw == false) {
      s->raw = raw_temporary;
      s->val_avg.reading(s->raw);
      s->last_raw = millis();
    }
  }

  // store the moving average filter with the raw 
  // sensor data to `val`. reset the filter.
  // then, add `val` to the ambient moving average
  // filter every 1 second.
  if(val_update) {
    noInterrupts();
      s->update_val = false;
    interrupts();
    s->val_prev = s->val;
    s->val = s->val_avg.getAvg();
    s->val_avg.reset();
    s->ambient_avg.reading(s->val);
    s->last_val = millis();
  }

  // store the ambient value every 60 seconds
  if(ambient_update) {
    //Serial << "\r\n\r\nhello ambient_update " << s->ambient_avg.getAvg() << "\r\n\r\n" << endl;
    noInterrupts();
      s->update_ambient = false;
    interrupts();
    s->ambient_prev = s->ambient;
    s->ambient = s->ambient_avg.getAvg();
    s->ambient_avg.reset();
    s->last_ambient = millis();

    for(uint8_t i=5; i>0; i--) {
      s->ambient_data[i] = s->ambient_data[i-1];
    }
    s->ambient_data[0] = s->ambient;

  }

}


void updateSensors() {

  dht_processed = false;

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    struct Sensor *s = all_sensors[i];
    if(s == NULL) continue;
    processSensors(s);
    s->updateSensor(s);
    printSensor(s);
  }

}


void printSensor(struct Sensor *s) {

  if(s == NULL) return;
  if(s->print == false) return;

  uint8_t raw_iteration;
  uint8_t raw_reload;
  uint8_t val_iteration;
  uint8_t val_reload;
  uint16_t ambient_iteration;
  uint16_t ambient_reload;

  // make it atomic by copying to local variables
  noInterrupts();
    raw_iteration = s->iteration_raw;
    raw_reload = s->reload_raw;
    val_iteration = s->iteration_val;
    val_reload = s->reload_val;
    ambient_iteration = s->iteration_ambient;
    ambient_reload = s->reload_ambient;
  interrupts();

  if(millis()-s->last_print >= s->print_frequency) {
    Serial << millis() << " " << s->name << " \t RAW: " << s->raw << " (" << raw_iteration << "/" << raw_reload << ")";
    Serial << " \t VAL: " << s->val << " (" << val_iteration << "/" << val_reload << ")";
    Serial << " \t AMBIENT: " << s->ambient << " (" << ambient_iteration << "/" << ambient_reload << ") ";
    
    if(s->ambient_data[5] != -99) {
      Serial << "math: " << s->ambient_data[5] - s->ambient_data[0] << endl;
    } else {
      Serial << endl;
    }
    
    s->last_print = millis();
  }

}


void initSensors() {

  timer_10Hz_config = NULL;
  dht_toggle = true;
  dht_processed = false;
  last_dht_processed = 0;

  dht.begin();

  for(uint8_t i=0; i<NUM_SENSORS; i++) {
    all_sensors[i] = NULL;
  }

  all_sensors[SENSOR_ID_LIGHT] = &sensor_light;
  initSensor_Light(all_sensors[SENSOR_ID_LIGHT]);

  all_sensors[SENSOR_ID_BATTERY] = &sensor_battery;
  initSensor_Battery(all_sensors[SENSOR_ID_BATTERY]);

  all_sensors[SENSOR_ID_SOUND] = &sensor_sound;
  initSensor_Sound(all_sensors[SENSOR_ID_SOUND]);

  if(TEMPERATURE_SENSOR_ENABLED) {
    all_sensors[SENSOR_ID_TEMPERATURE] = &sensor_temperature;
    initSensor_Temperature(all_sensors[SENSOR_ID_TEMPERATURE]);
  }

  if(HUMIDITY_SENSOR_ENABLED) {
    all_sensors[SENSOR_ID_HUMIDITY] = &sensor_humidity;
    initSensor_Humidity(all_sensors[SENSOR_ID_HUMIDITY]);
  }

  
  // set up timer 1 for every 0.1 second
  // params: timer #1, prescaler amount, count up (true)
  timer_10Hz_config = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer_10Hz_config, &Timer_10Hz_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer_10Hz_config, 500, true);
  timerAlarmEnable(timer_10Hz_config);

  if(RTOS_ENABLED) {

    Mutex_SENSORS = xSemaphoreCreateMutex();

    // core 0 has task watchdog enabled to protect wifi service etc
    // core 1 does not have watchdog enabled
    // can do this if wdt gives trouble: disableCore0WDT();
    xTaskCreatePinnedToCore(
                      Task_SENSORS_code,     // task function
                      "Task_SENSORS",        // name of task
                      STACK_SENSORS,         // stack size of task
                      NULL,                  // parameter of the task
                      PRIORITY_SENSORS_MID,  // priority of the task (low number = low priority)
                      &Task_SENSORS,         // task handle to keep track of created task
                      TASK_CORE_SENSORS);    // pin task to core

  }

}




// ----------------------------------
// --------- battery sensor ---------
// ----------------------------------

uint16_t getSensor_Battery(struct Sensor *s) {
  return analogRead(BATT_SIG_PIN);
}


void updateSensor_Battery(struct Sensor *s) {

  if(s == NULL) return;

  // logic for battery threshold can go here
  // future update

}


void initSensor_Battery(struct Sensor *s) {

  s->id = SENSOR_ID_BATTERY;
  s->name = "Battery";
  s->print = false;
  s->print_frequency = 1000;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Battery;
  s->updateSensor = updateSensor_Battery;

  // battery sensor has no trigger or ambient change callbacks

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_battery.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_battery.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

// ----------------------------------
// ----------------------------------



// ----------------------------------
// -------- humidity sensor ---------
// ----------------------------------

uint16_t getSensor_Humidity(struct Sensor *s) {
  
  if(s == NULL) return 999;

  int humid_raw = 0;
  float h = 99.9;

  noInterrupts();
    h = dht.readHumidity();
  interrupts();

  if(isnan(h)) {
    humid_raw = 999;
    if(s->print) Serial << "Failed to read from DHT sensor (humidity)" << endl;
  } else {
    humid_raw = (int)(h*10);
    //if(s->print) Serial << "New humidity reading: " << h << ", " << humid_raw << endl;
  }

  return (uint16_t)humid_raw;
}


void updateSensor_Humidity(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger on raw data
  if(abs(s->raw - s->raw_prev) >= HUMIDITY_CHANGE_THRESH // see if the change is great enough
     && s->last_raw != -99 && s->raw_prev != 0) {
    if(s->raw > s->raw_prev) { // see if going from wet to dry or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- trigger on ambient data
  if(s->ambient_data[5] != -99) { // check that the data has been populated

    // compare the data from 10 mins ago to now
    // and do this comparison every 2 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= HUMIDITY_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60*2) // 2 min wait
      && s->ambient_data[0] != 0 && s->ambient_data[5] != 0) { 
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Humidity(struct Sensor *s) {

  s->id = SENSOR_ID_HUMIDITY;
  s->name = "Humidity";
  s->print = false;
  s->print_frequency = 3000;
  
  s->reload_raw = 1*10*3;          // every 3 seconds
  s->reload_val = 10*6*20;         // every 20 seconds
  s->reload_ambient = 600*2;       // every 120 seconds

  // functions
  s->getRawData = getSensor_Humidity;
  s->updateSensor = updateSensor_Humidity;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_humidity.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_humidity.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

// ----------------------------------
// ----------------------------------


// ----------------------------------
// --------- light sensor -----------
// ----------------------------------

uint16_t getSensor_Light(struct Sensor *s) {
  return analogRead(LDR_PIN);
}


void updateSensor_Light(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger
  if(abs(s->val - s->val_prev) >= LIGHT_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99 && s->val_prev != 0) {
    if(s->val > s->val_prev) { // see if going from dark to light or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- ambient check
  if(s->ambient_data[5] != -99) { // see that the data has been populated

    // compare the data from 5 mins ago to now
    // and do this comparison every 1 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= LIGHT_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60) ) { // 1 min wait
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Light(struct Sensor *s) {

  s->id = SENSOR_ID_LIGHT;
  s->name = "Light";
  s->print = false;
  s->print_frequency = 1000;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Light;
  s->updateSensor = updateSensor_Light;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_light.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_light.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

// ----------------------------------
// ----------------------------------


// ----------------------------------
// --------- sound sensor -----------
// ----------------------------------

uint16_t getSensor_Sound(struct Sensor *s) {
  return analogRead(MIC_PIN);
}


void updateSensor_Sound(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger
  if(abs(s->val - s->val_prev) >= SOUND_CHANGE_THRESH // see if the change is great enough
    && s->last_val != -99 && s->val_prev != 0) {
    if(s->val > s->val_prev) { // see if going from loud to quiet or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 500) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- ambient check
  if(s->ambient_data[5] != -99) { // see that the data has been populated

    // compare the data from 5 mins ago to now
    // and do this comparison every 1 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= SOUND_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60) ) { // 1 min wait
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Sound(struct Sensor *s) {

  s->id = SENSOR_ID_SOUND;
  s->name = "Sound";
  s->print = false;
  s->print_frequency = 1000;
  
  s->reload_raw = 1;          // every 0.1 seconds
  s->reload_val = 10;         // every 1 seconds
  s->reload_ambient = 600;    // every 60 seconds

  // functions
  s->getRawData = getSensor_Sound;
  s->updateSensor = updateSensor_Sound;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_sound.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_sound.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

// ----------------------------------
// ----------------------------------


// ----------------------------------
// ------ temperature sensor --------
// ----------------------------------

uint16_t getSensor_Temperature(struct Sensor *s) {
  
  if(s == NULL) return 999;

  int temp_raw = 0;
  float t = 99.9;

  noInterrupts();
    t = dht.readTemperature();
  interrupts();

  if(isnan(t)) {
    temp_raw = 999;
    if(s->print) Serial << "Failed to read from DHT sensor (temperature)" << endl;
  } else {
    temp_raw = (int)(t*10);
    if(s->print) Serial << "New temperature reading: " << t << ", " << temp_raw << endl;
  }

  return (uint16_t)temp_raw;
}


void updateSensor_Temperature(struct Sensor *s) {

  if(s == NULL) return;

  // -- trigger on raw data
  if(abs(s->raw - s->raw_prev) >= TEMPERATURE_CHANGE_THRESH // see if the change is great enough
     && s->last_raw != -99 && s->raw_prev != 0) {
    if(s->raw > s->raw_prev) { // see if going from cold to warm or vice versa 
      if(s->trigger_dir != false || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = false;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    } else {
      if(s->trigger_dir != true || millis()-s->last_sensor_trigger >= 2000) { // avoid double triggers
        s->trigger_dir = true;
        s->trig_count++;
        if(s->onSensorChangeCallback) s->onSensorChangeCallback(s, s->trigger_dir);
      }
    }
    s->last_sensor_trigger = millis();
  }
  // --

  // -- trigger on ambient data
  if(s->ambient_data[5] != -99) { // check that the data has been populated

    // compare the data from 10 mins ago to now
    // and do this comparison every 2 min
    if(abs( s->ambient_data[5] - s->ambient_data[0] ) >= TEMPERATURE_AMBIENT_THRESH 
      && millis()-s->last_ambient_trigger >= (1000*60*2) // 2 min wait
      && s->ambient_data[0] != 0 && s->ambient_data[5] != 0) { 
      if(s->onSensorAmbientChangeCallback) s->onSensorAmbientChangeCallback(s, s->ambient_data[5] - s->ambient_data[0]);
      s->last_ambient_trigger = millis();
    }

  }
  // --

}


void initSensor_Temperature(struct Sensor *s) {

  s->id = SENSOR_ID_TEMPERATURE;
  s->name = "Temperature";
  s->print = false;
  s->print_frequency = 3000;
  
  s->reload_raw = 1*10*3;          // every 3 seconds
  s->reload_val = 10*6*20;         // every 20 seconds
  s->reload_ambient = 600*2;       // every 120 seconds

  // functions
  s->getRawData = getSensor_Temperature;
  s->updateSensor = updateSensor_Temperature;

  s->last_val = -99;
  s->last_ambient = -99;

  // the moving averages are init'ed in the struct constructor
  //sensor_temperature.val_avg(SENSOR_MOVING_AVG_VAL_WINDOW);
  //sensor_temperature.ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW);

  s->val_avg.begin();
  s->ambient_avg.begin();

  for(uint8_t j=0; j<6; j++) {
    s->ambient_data[j] = -99;
  }
}

// ----------------------------------
// ----------------------------------


void Task_SENSORS_code(void * pvParameters) {
  while(1) {

    // take mutex prior to critical section
    if(xSemaphoreTake(Mutex_SENSORS, (TickType_t)10) == pdTRUE) {
      
      updateSensors();

      if(DEBUG_SENSORS_RTOS == true && millis()-last_sensors_rtos_print >= 1000) {
        Serial << "sensors stack watermark: " << uxTaskGetStackHighWaterMark( NULL );
        Serial << "\t\tavailable heap: " << xPortGetFreeHeapSize() << endl; //vPortGetHeapStats().xAvailableHeapSpaceInBytes
        last_sensors_rtos_print = millis();
      }

      // give mutex after critical section
      xSemaphoreGive(Mutex_SENSORS);
    }
    
    //vTaskDelay(1);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    if(TASK_FREQ_SENSORS != 0) vTaskDelayUntil( &xLastWakeTime, TASK_FREQ_SENSORS );
  }
  // task destructor prevents the task from doing damage to the other tasks in case a task jumps its stack
  vTaskDelete(NULL);
}


void setSensorsTaskPriority(uint8_t p) {
  
  if(!RTOS_ENABLED) return;

  uint8_t prev_priority = uxTaskPriorityGet(Task_SENSORS);
  vTaskPrioritySet(Task_SENSORS, p);
  if (DEBUG_SENSORS_RTOS) Serial << "changed SENSORS task priority - new: " << p << " prev: " << prev_priority << endl;

}

