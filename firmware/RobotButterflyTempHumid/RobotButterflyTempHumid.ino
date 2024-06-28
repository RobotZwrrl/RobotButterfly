/*
 * Robot Butterfly Temperature & Humidity
 * -------------------------------------
 * Sample the DHT11 sensor, store data, analyse the changes
 * Behaviour functions are called where custom animations can be added
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 *
 * 15 Jun 2024 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include <Streaming.h>
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg
#include <DHT11.h>   // DHT11 library by Dhruba Saha


// ----- timer every 1.5 seconds
// (DHT11 reliably gives readings every 1.5 seconds, 
//  as seen through validation testing)
hw_timer_t *timer0_cfg = NULL;
hw_timer_t *timer1_cfg = NULL;
 
volatile bool sample_dht11 = false;
volatile bool store_window = false;

void IRAM_ATTR Timer0_ISR() {
  sample_dht11 = true;
}

void IRAM_ATTR Timer1_ISR() {
  store_window = true;
}
// -----

// ----- temperature & humidity
DHT11 dht11(DHT11_PIN);

#define DATA_LOG_STORAGE 240 // 60 for 15 mins, 120 for 30 mins, 240 for 60 mins

int temp_raw = 0;
int humid_raw = 0;
struct DataPoint {
  long record_time;
  uint16_t record_num;
  int temp_avg;
  int humid_avg;
};
struct DataPoint data_log[DATA_LOG_STORAGE];
uint16_t data_count = 0;
int16_t log_ind = -1;
uint8_t log_current = 0;
uint8_t log_previous = 0;
movingAvg temp_avg(15);
movingAvg humid_avg(15);
bool go = true;
// ----- 


void setup() {
  Serial.begin(9600);

  pinMode(LED_HEARTBEAT_PIN, OUTPUT);
  digitalWrite(LED_HEARTBEAT_PIN, HIGH);
  pinMode(LED_COMMS_PIN, OUTPUT);
  digitalWrite(LED_COMMS_PIN, LOW);

  Serial << "\r\n";

  dht11.setDelay(0); // their library has a blocking delay, set to 0

  // set up timer 0 for every 1.5 second
  // params: timer #0, prescaler amount, count up (true)
  timer0_cfg = timerBegin(0, 16000, true);
  timerAttachInterrupt(timer0_cfg, &Timer0_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer0_cfg, 7500, true);
  timerAlarmEnable(timer0_cfg);

  // set up timer 1 for every 15 second
  // params: timer #1, prescaler amount, count up (true)
  timer1_cfg = timerBegin(1, 16000, true);
  timerAttachInterrupt(timer1_cfg, &Timer1_ISR, true);
  // params: timer, tick count, auto-reload (true)
  timerAlarmWrite(timer1_cfg, 75000, true);
  timerAlarmEnable(timer1_cfg);

  temp_avg.begin();
  humid_avg.begin();

}


void loop() {

  if(go) updateTempHumid();

  if(Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'd': // data log csv
        Serial << "record_num,record_time,temp_avg,humid_avg" << endl;
        for(uint8_t i=0; i<DATA_LOG_STORAGE; i++) {
          struct DataPoint d = data_log[i];
          Serial << d.record_num << "," << d.record_time << ",";
          Serial << d.temp_avg << "," << d.humid_avg << endl;
        }
      break;
      case 's': // stop
        Serial << "stop" << endl;
        go = false;
        timerStop(timer0_cfg);
        timerStop(timer1_cfg);
        temp_avg.reset();
        humid_avg.reset();
      break;
      case 'g': // go
        Serial << "go" << endl;
        go = true;
        timerRestart(timer0_cfg);
        timerRestart(timer1_cfg);
        timerStart(timer0_cfg);
        timerStart(timer1_cfg);
      break;
      case 'a': // analysis
        analysisTempHumid();
      break;
      case 'h':
        Serial << "help" << endl;
        Serial << "s: stop" << endl;
        Serial << "g: go" << endl;
        Serial << "d: data log csv print out" << endl;
        Serial << "a: analysis" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}


void behaviourTemperature(int delta_temp) {
  Serial << "---------" << endl;
  Serial << "temperature change! " << delta_temp << endl;
  Serial << "---------" << endl;
  // do something here :)
}


void behaviourHumidity(int delta_humid) {
  Serial << "---------" << endl;
  Serial << "humidity change! " << delta_humid << endl;
  Serial << "---------" << endl;
  // do something here :)
}


void updateTempHumid() {

  bool sample_dht11_copy = false;
  bool store_window_copy = false;

  noInterrupts(); // same as portDISABLE_INTERRUPTS();
  sample_dht11_copy = sample_dht11;
  store_window_copy = store_window;
  interrupts(); // same as portENABLE_INTERRUPTS();

  if(sample_dht11_copy) {

    if(dht11.readTemperatureHumidity(temp_raw, humid_raw) != 0) {
      Serial << millis() << " ERROR" << endl;
      Serial << "---------" << endl;
      sample_dht11 = false;
      return;
    }

    if(temp_raw == 0 && humid_raw == 0) { // bad reading
      Serial << millis() << " BAD READING" << endl;
      Serial << "---------" << endl;
      sample_dht11 = false;
      return;
    }

    // add to the moving average
    temp_avg.reading(temp_raw);
    humid_avg.reading(humid_raw);

    Serial << millis() << "   ";
    Serial << "#: " << temp_avg.getCount();
    Serial << "   Temp avg: " << temp_avg.getAvg() << " raw: " << temp_raw << "°C";
    Serial << "   Humid avg: " << humid_avg.getAvg() << " raw: " << humid_raw << "%";
    Serial << endl;
    Serial << "Data log count: " << log_ind+1 << "/" << DATA_LOG_STORAGE << " Total: " << data_count << endl;
    Serial << "---------" << endl;

    sample_dht11 = false;
  }

  // store this window every 15 seconds as per timer
  // that's ~10 readings (which are done every 1.5 seconds)
  if(store_window_copy) {
    
    log_previous = log_ind;
    if(log_ind == -1) log_previous = 0; // edge case
    if(log_ind >= DATA_LOG_STORAGE-1) log_ind = -1;
    log_ind++;
    log_current = log_ind;

    Serial << "current ind: " << log_current << " prev ind: " << log_previous << endl;

    data_log[log_ind].temp_avg = temp_avg.getAvg();
    data_log[log_ind].humid_avg = humid_avg.getAvg();;
    data_log[log_ind].record_num = data_count;
    data_log[log_ind].record_time = millis();
    
    data_count++;

    // reset
    temp_avg.reset();
    humid_avg.reset();
    store_window = false;

    analysisTempHumid();
  }

}


void analysisTempHumid() {

  if(log_previous == 0 && log_current == 0) return; // not enough data yet

  struct DataPoint d_current = data_log[log_current];
  struct DataPoint d_previous = data_log[log_previous];

  if(d_current.temp_avg == 0 || d_previous.temp_avg == 0) return; // not enough data yet
  if(d_current.humid_avg == 0 || d_previous.humid_avg == 0) return; // not enough data yet

  int delta_temp = d_current.temp_avg - d_previous.temp_avg;
  int delta_humid = d_current.humid_avg - d_previous.humid_avg;

  bool no_change = true;

  if( abs(delta_temp) >= 1 ) {
    behaviourTemperature(delta_temp);
    no_change = false;
  }

  if( abs(delta_humid) >= 1 ) {
    behaviourHumidity(delta_humid);
    no_change = false;
  }

  if(no_change) {
    Serial << "analysis: no change" << endl;
  }

}


void heartbeatUpdate() {
  uint16_t slice = millis() % 1000;
  digitalWrite(LED_HEARTBEAT_PIN, slice < 100 || (slice > 200 && slice < 300));
}



/*
 * Additional notes:
 * 
 * dht11 lib requires TIMEOUT_DURATION = 0; in DHT11.h
 *
 * timers info
 * https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/
 * ESP32s have 4 hardware timers
 * general-purpose 64-bit up/down counter with a 16-bit prescaler
 * max tick count: 18446744073709551615
 * max prescaler: 65536
 */

