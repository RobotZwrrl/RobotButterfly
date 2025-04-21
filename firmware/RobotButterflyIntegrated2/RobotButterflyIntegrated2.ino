/*
 * Robot Butterfly Integrated 2
 * ----------------------------
 * Sandbox sketches together as one sketch
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 2.0.4 (<-- newer available)
 *
 * 21 Apr 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "StateMachine_Params.h"
#include "Sound_Params.h"
#include "ServoAnim_Params.h"
#include "IMU_Params.h"
#include "Buttons_Params.h"
#include "Sensor_Params.h"
#include "Proximity_Params.h"
#include "NeoAnimations_Params.h"
#include "pitches.h"
#include <Streaming.h>
#include <ESP32Servo.h>      // 3.0.6 https://github.com/madhephaestus/ESP32Servo 
#include "ServoEasing.hpp"   // 3.4.0 https://github.com/ArminJo/ServoEasing
#include <movingAvg.h>   // 2.3.2 https://github.com/JChristensen/movingAvg
#include "DHT.h"         // 1.4.6 https://github.com/adafruit/DHT-sensor-library & 1.1.15 https://github.com/adafruit/Adafruit_Sensor 
#include <HCSR04.h>      // 1.1.3 https://github.com/d03n3rfr1tz3/HC-SR04 
#include "MPU6050.h"    // https://github.com/ElectronicCats/mpu6050/ 
#include <Adafruit_NeoPixel.h>


// ------------ states -----------
// this is where you can order the behaviours
enum StatesMachine {
  STATE1,
  STATE2,
  STATE3,
  STATE4,
  STATE5,
  STATE6,
  STATE7,
  STATE8
};

uint8_t CURRENT_STATE;
uint8_t PREV_STATE;
long last_state_change = 0;
long last_state_print = 0;

volatile bool enter_state = false;
volatile bool update_state = false;
volatile bool new_print = false;
volatile bool new_enter = false;
volatile bool new_update = false;
long t_transition = 0;
long t_enter = 0;
long t_delta = 0;

hw_timer_t *timer_state_cfg = NULL;

void IRAM_ATTR Timer_State_ISR() {
  enter_state = false;
  update_state = true;
  new_print = true;
  new_update = true;
}
// -------------------------------

// ----------- buzzer api ------------
void playSound(uint8_t id);
void soundDoneCallback(uint8_t id);
// -----------------------------------


// ------------ sound id -------------
enum soundID {
  SOUND_ALERT_STARTUP,
  SOUND_ALERT_ALERT,
  SOUND_ALERT_SLEEP,
  SOUND_ALERT_SNEEZE,
  SOUND_ALERT_MISSION_COMPLETE,
  SOUND_ALERT_MOONLIGHT_MODE,
  SOUND_ALERT_CHIRPY,
  SOUND_ALERT_WIFI_CONNECT,
  SOUND_ALERT_WIFI_DISCONNECT,
  SOUND_ALERT_MQTT_SEND,
  SOUND_ALERT_MQTT_RECEIVE,

  SOUND_SMELL_FLOWER,
  SOUND_SMELL_ROSE,
  SOUND_SMELL_WILDFLOWER,
  SOUND_SMELL_LAVENDER,
  SOUND_SMELL_DAISY,
  SOUND_SMELL_SUNFLOWER,

  SOUND_FLUTTER_MEMORY,
  SOUND_FLUTTER_JOY,
  SOUND_FLUTTER_CALM,
  SOUND_FLUTTER_SURPRISE,
  SOUND_FLUTTER_CONFUSED,
  SOUND_FLUTTER_SLEEPY,
  SOUND_FLUTTER_GRATEFUL
};
// -----------------------------------

// ----------- servo motors -----------
ServoEasing s1;
ServoEasing s2;

struct ServoMotor {
  ServoEasing motor;
  uint16_t home_pos;
  uint16_t up_pos;
  uint16_t down_pos;
  uint16_t current_pos;
  uint16_t target_pos;
  bool active;
  bool calibrated;
  String name;
};

enum ServoModeInit {
  SERVO_MODE_INIT_BOTH,
  SERVO_MODE_INIT_LEFT,
  SERVO_MODE_INIT_RIGHT
};

struct ServoMotor wing_left;
struct ServoMotor wing_right;

bool initialised_servos = false;
// -----------------------------------


// ----------- servo animation -----------
enum servoAnimName {
  SERVO_ANIM_NONE,
  SERVO_ANIM_GENTLE,
  SERVO_ANIM_SWAY,
  SERVO_ANIM_SOARING,
  SERVO_ANIM_TOUCHGRASS,
  SERVO_ANIM_SWOOSH,
  SERVO_ANIM_PARTY,
  SERVO_ANIM_FLUTTER,
  SERVO_ANIM_RANGE,
  SERVO_ANIM_POSITION
};

enum servoAnimType {
  SERVO_ANIM_ALERT,
  SERVO_ANIM_HOME
};

enum servoAnimFlutterWings {
  SERVO_ANIM_FLUTTER_WINGS_BOTH_HOME,
  SERVO_ANIM_FLUTTER_WINGS_BOTH_UP,
  SERVO_ANIM_FLUTTER_WINGS_LEFT_HOME,
  SERVO_ANIM_FLUTTER_WINGS_LEFT_UP,
  SERVO_ANIM_FLUTTER_WINGS_RIGHT_HOME,
  SERVO_ANIM_FLUTTER_WINGS_RIGHT_UP
};

enum servoAnimFlutterPos {
  SERVO_ANIM_FLUTTER_POS_NONE,
  SERVO_ANIM_FLUTTER_POS_HOME,
  SERVO_ANIM_FLUTTER_POS_UP,
  SERVO_ANIM_FLUTTER_POS_DOWN
};

enum servoAnimRangeSpan {
  SERVO_ANIM_RANGE_DOWN_UP,
  SERVO_ANIM_RANGE_HOME_UP,
  SERVO_ANIM_RANGE_ALT_HOME_UP
};

enum servoAnimPosition {
  SERVO_ANIM_POSITION_DOWN,
  SERVO_ANIM_POSITION_HOME,
  SERVO_ANIM_POSITION_UP
};

void runServoAnim_none(struct ServoAnimation *animation);
void runServoAnim_gentle(struct ServoAnimation *animation);
void runServoAnim_sway(struct ServoAnimation *animation);
void runServoAnim_soaring(struct ServoAnimation *animation);
void runServoAnim_touchgrass(struct ServoAnimation *animation);
void runServoAnim_swoosh(struct ServoAnimation *animation);
void runServoAnim_party(struct ServoAnimation *animation);
void runServoAnim_flutter(struct ServoAnimation *animation);
void runServoAnim_range(struct ServoAnimation *animation);
void runServoAnim_position(struct ServoAnimation *animation);
typedef void (*ServoAnimationFunction)(ServoAnimation*); // function pointer type that accepts a ServoAnimation pointer

struct ServoAnimation {
  uint8_t id;
  bool active;
  uint8_t type;
  uint8_t velocity;   // degrees per second

  uint8_t num_frames;
  uint16_t frame_delay;
  int frame_index;
  long last_frame;
  
  int num_repeats;
  uint16_t repeat_count;
  uint16_t repeat_delay;
  long last_repeat;

  long duration;
  long start_time;

  bool dir;
  int helper1;
  int helper2;
  int helper3;

  ServoAnimationFunction function;  // function pointer
};

ServoAnimation servo_animation_home;
ServoAnimation servo_animation_alert;
// -----------------------------------

// ------------ callbacks ------------
void proximityTriggerCallback(struct Proximity *p);
// ------------------------------------

// ----------- ultrasonic -------------
struct Proximity {
  bool initialised;
  long last_print;
  long last_trigger;
  uint8_t threshold;

  volatile bool update_raw;
  volatile uint8_t iteration_raw;
  volatile uint8_t reload_raw;
  uint16_t raw;
  uint16_t raw_prev;
  long last_raw;

  uint16_t val;
  uint16_t val_prev;
  movingAvg val_avg;

  Proximity() 
  : initialised(false), last_print(0), last_trigger(0), threshold(0),
    update_raw(false), iteration_raw(0), reload_raw(0), raw(0), raw_prev(0), last_raw(0),
    val_avg(PROXIMITY_MOVING_AVG_WINDOW, true)
{}

};

static struct Proximity ultrasonic;
// ------------------------------------

// ------------ callbacks ------------
void sensorLightChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorLightAmbientChangeCallback(struct Sensor *s, int change);
void sensorSoundChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorSoundAmbientChangeCallback(struct Sensor *s, int change);
void sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change);
void sensorHumidityChangeCallback(struct Sensor *s, bool trigger_dir);
void sensorHumidityAmbientChangeCallback(struct Sensor *s, int change);
// ------------------------------------

// -------------- vars ---------------
DHT dht(DHT11_PIN, DHT11);

uint16_t getSensor_Light(struct Sensor *s);
void updateSensor_Light(struct Sensor *s);
uint16_t getSensor_Battery(struct Sensor *s);
void updateSensor_Battery(struct Sensor *s);
uint16_t getSensor_Sound(struct Sensor *s);
void updateSensor_Sound(struct Sensor *s);
uint16_t getSensor_Temperature(struct Sensor *s);
void updateSensor_Temperature(struct Sensor *s);
uint16_t getSensor_Humidity(struct Sensor *s);
void updateSensor_Humidity(struct Sensor *s);
typedef uint16_t (*SensorDAQFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
typedef void (*SensorUpdateFunction)(Sensor*); // function pointer type that accepts a Sensor pointer
// ------------------------------------

// --------------- sensor ----------------
hw_timer_t *timer_10Hz_config = NULL;

enum SensorIDs {
  SENSOR_ID_LIGHT,  
  SENSOR_ID_BATTERY,
  SENSOR_ID_SOUND,
  SENSOR_ID_TEMPERATURE,
  SENSOR_ID_HUMIDITY
};

struct Sensor {
  uint8_t id;
  String name;
  bool print;
  long print_frequency;
  long last_print;
  uint16_t trig_count;
  bool trigger_dir;
  long last_sensor_trigger;
  long last_ambient_trigger;

  // collected every 0.1 seconds
  volatile bool update_raw;
  volatile uint8_t iteration_raw;
  volatile uint8_t reload_raw;
  uint16_t raw;
  uint16_t raw_prev;
  long last_raw;

  // averaged over 1 seconds
  volatile bool update_val;
  volatile uint8_t iteration_val;
  volatile uint8_t reload_val;
  uint16_t val;
  uint16_t val_prev;
  long last_val;
  movingAvg val_avg;

  // averaged over 60 seconds
  volatile bool update_ambient;
  volatile uint16_t iteration_ambient;
  volatile uint16_t reload_ambient;
  uint16_t ambient;
  uint16_t ambient_prev;
  long last_ambient;
  movingAvg ambient_avg;
  int ambient_data[6];

  // ---
  SensorDAQFunction getRawData;        // function pointer
  SensorUpdateFunction updateSensor;   // function pointer

  // constructor
  Sensor()
  : id(0), name(""), print(true), print_frequency(0), last_print(0), trig_count(0), trigger_dir(false), last_sensor_trigger(0), last_ambient_trigger(0),
    update_raw(false), iteration_raw(0), reload_raw(0), raw(0), raw_prev(0), last_raw(0),
    update_val(false), iteration_val(0), reload_val(0), val(0), val_prev(0), last_val(0),
    val_avg(SENSOR_MOVING_AVG_VAL_WINDOW, true),
    update_ambient(false), iteration_ambient(0), reload_ambient(0), ambient(0), ambient_prev(0), last_ambient(0),
    ambient_avg(SENSOR_MOVING_AVG_AMBIENT_WINDOW, true),
    ambient_data(),
    getRawData(NULL), updateSensor(NULL)
  {}

};

static struct Sensor sensor_temperature;
static struct Sensor sensor_humidity;
static struct Sensor sensor_light;
static struct Sensor sensor_sound;
static struct Sensor sensor_battery;

static struct Sensor *all_sensors[NUM_SENSORS];

static bool dht_toggle = true;
static bool dht_processed = false;
static long last_dht_processed = 0;
// ------------------------------------

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


  // refresh ultrasonic every 0.5 seconds
  if(ultrasonic.iteration_raw >= ultrasonic.reload_raw-1) {
    ultrasonic.update_raw = true;
    ultrasonic.iteration_raw = 0;
  } else {
    if(ultrasonic.update_raw == false) {
      ultrasonic.iteration_raw++;
    }
  }


}
// ------------------------------------

// ----------- neoanim callbacks -----------
void callback_NeoAnimDone(struct NeoAnimation *a);
void callback_NeoAnimLoop(struct NeoAnimation *a);
// -----------------------------------


// ----------- neopixel colours -----------
// go with 400 kHz as 800 kHz would leave pixel colour ghosts
Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

enum pastelIndex {
  NEO_RED,
  NEO_ORANGE,
  NEO_GOLDEN_YELLOW,
  NEO_CANARY_YELLOW,
  NEO_GREEN,
  NEO_SKY_BLUE,
  NEO_BLUE,
  NEO_CYAN,
  NEO_PURPLE,
  NEO_LAVENDER,
  NEO_PINK,
  NEO_MAGENTA,
  NEO_WARM_WHITE,
  NEO_WHITE,
  NEO_OFF
};

String pastelNames[NUM_PALETTE_COLOURS] = {
  "Red",
  "Orange",
  "Golden Yellow",
  "Canary Yellow",
  "Green",
  "Sky Blue",
  "Blue",
  "Cyan",
  "Purple",
  "Lavender",
  "Pink",
  "Magenta",
  "Warm White",
  "White",
  "Off"
};

// these are the colour settings when the 
// brightness is set to 20, which is visible
// during daylight outdoors
uint16_t pastelColours[NUM_PALETTE_COLOURS][3] = {
    {  100,  180,  250},   // red
    { 3500,  190,  250},   // orange
    { 6622,  190,  250},   // golden yellow
    { 9000,  190,  250},   // canary yellow
    {15545,  170,  250},   // green
    {32845,   50,  250},   // sky blue
    {43691,  120,  250},   // blue
    {27306,  120,  250},   // cyan
    {51613,  130,  250},   // purple
    {56613,   40,  250},   // lavendar
    {  359,   90,  250},   // pink
    {63535,  140,  250},   // magenta
    { 6561,  140,  250},   // warm white
    { 8192,   90,  250},   // white
    {    0,    0,    0},   // off
};

uint32_t colourPalette[NUM_PALETTE_COLOURS];
// -----------------------------------

// ------- neopixel animation --------
enum neoAnimName {
  NEO_ANIM_NONE,
  NEO_ANIM_POLKADOT,
  NEO_ANIM_SQUIGGLE,
  NEO_ANIM_RANGE,
  NEO_ANIM_FUNKY,
  NEO_ANIM_ZWOOP,
  NEO_ANIM_SPRINKLE,
  NEO_ANIM_RAINBOW,
  NEO_ANIM_RAINBOW_ALL,
  NEO_ANIM_BOUNCE,
  NEO_ANIM_BREATHE,
  NEO_ANIM_CYCLE8,
  NEO_ANIM_CYCLE3,
  NEO_ANIM_AMBIANCE,
  NEO_ANIM_UNO
};

enum neoAnimType {
  NEO_ANIM_ALERT,
  NEO_ANIM_HOME
};

enum neoAnimAmbianceID {
  NEO_ANIM_AMBIANCE_SPRING,
  NEO_ANIM_AMBIANCE_SUMMER,
  NEO_ANIM_AMBIANCE_AUTUMN,
  NEO_ANIM_AMBIANCE_WINTER,
  NEO_ANIM_AMBIANCE_TUTTI_FRUITI
};

void runNeoAnim_none(struct NeoAnimation *animation);
void runNeoAnim_polkadot(struct NeoAnimation *animation);
void runNeoAnim_squiggle(struct NeoAnimation *animation);
void runNeoAnim_range(struct NeoAnimation *animation);
void runNeoAnim_funky(struct NeoAnimation *animation);
void runNeoAnim_zwoop(struct NeoAnimation *animation);
void runNeoAnim_sprinkle(struct NeoAnimation *animation);
void runNeoAnim_rainbow(struct NeoAnimation *animation);
void runNeoAnim_rainbow_all(struct NeoAnimation *animation);
void runNeoAnim_bounce(struct NeoAnimation *animation);
void runNeoAnim_breathe(struct NeoAnimation *animation);
void runNeoAnim_cycle8(struct NeoAnimation *animation);
void runNeoAnim_cycle3(struct NeoAnimation *animation);
void runNeoAnim_ambiance(struct NeoAnimation *animation);
void runNeoAnim_uno(struct NeoAnimation *animation);
typedef void (*AnimationFunction)(NeoAnimation*); // function pointer type that accepts a NeoAnimation pointer

struct NeoAnimation {
  uint8_t id;
  bool active;
  uint8_t type;

  uint8_t colour_primary;    // enum of the colour
  uint8_t colour_secondary;  // enum of the colour

  uint8_t num_frames;
  int frame_delay;
  uint8_t frame_index;
  long last_frame;
  
  int num_repeats;
  uint16_t repeat_count;
  uint16_t repeat_delay;
  long last_repeat;

  long duration;
  long start_time;

  bool dir;
  int helper1;
  int helper2;
  int helper3;

  AnimationFunction function;  // function pointer
};

NeoAnimation neo_animation_home;
NeoAnimation neo_animation_alert;
// -----------------------------------

// ------------ callbacks ------------
void imuStateChangeCallback(uint8_t s);
void imuOrientationChangeCallback(uint8_t o);
void imuPoseChangeCallback(uint8_t p);
void imuEventDetectedCallback(uint8_t e);
// ------------------------------------

// --------------- imu ----------------
hw_timer_t *timer_10Hz_imu_cfg = NULL;
volatile bool new_avg_sample = false;

MPU6050 mpu;

struct IMUData {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  long last_data;
};

static struct IMUData imu;
static struct IMUData imu_prev;
static struct IMUData imu_avg;
static struct IMUData imu_home;
static struct IMUData imu_delta_home_avg;

enum IMUStates {
  IMU_SETTLE,
  IMU_CALIBRATE_HOME,
  IMU_ACTIVE,
  IMU_INACTIVE
};

uint8_t IMU_STATE = IMU_INACTIVE;
uint8_t IMU_STATE_PREV = IMU_INACTIVE;

enum IMUOrientations {
  IMU_TABLETOP,
  IMU_HANG,
  IMU_UNKNOWN
};

uint8_t IMU_ORIENTATION = IMU_UNKNOWN;
uint8_t IMU_ORIENTATION_PREV = IMU_UNKNOWN;
long last_orientation_change = 0;

enum IMUPoses {
  IMU_Pose_Tilt_L,
  IMU_Pose_Tilt_R,
  IMU_Pose_Tilt_Fwd,
  IMU_Pose_Tilt_Bwd,
  IMU_Pose_Home,
  IMU_Pose_NA
};

uint8_t IMU_POSE = IMU_Pose_NA;
uint8_t IMU_POSE_PREV = IMU_Pose_NA;
long last_pose_detected = 0;

bool EVENT_DETECTED = false;
long last_event_detected = 0;
long last_event_score_clear = 0;
int event_score = 0;

movingAvg imu_avg_data_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_data_gz(IMU_MOVING_AVG_WINDOW);

movingAvg imu_avg_home_ax(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_ay(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_az(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gx(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gy(IMU_MOVING_AVG_WINDOW);
movingAvg imu_avg_home_gz(IMU_MOVING_AVG_WINDOW);

long last_imu_calib_print = 0;
long last_home_calibration = 0;
long settle_start = 0;
long calibration_start = 0;
long last_score_clear = 0;
int home_recalibrate_score = 0;
long last_imu_stats_print = 0;

bool IMU_PRINT_RAW = false;
bool IMU_PRINT_DATA_AVG = false;
bool IMU_PRINT_DELTA_HOME_AVG = false; // usually true during dev
bool IMU_PRINT_DELTA_TIME_AVG = false;
bool IMU_PRINT_STATS = true; // usually true during testing
// ------------------------------------

// ------------- imu isr --------------
void IRAM_ATTR Timer_10Hz_imu_ISR() { // every 0.1 seconds
  new_avg_sample = true;
}
// ------------------------------------

// ----------- buttons -----------
enum ButtonNames {
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_BOTH
};

enum ButtonStates {
  BUTTON_IDLE,
  BUTTON_RELEASED,
  BUTTON_BOTH_HOLD,
  BUTTON_HOLD,
  BUTTON_PRESSED,
  BUTTON_CLICK
};

struct Button {
  uint8_t state;
  uint8_t state_prev;
  bool pressed;
  bool flag_pressed;
  bool flag_released;
  bool flag_state_change;
  bool flag_button_hold_notif;
  long press_time;
  long release_time;
  long release_both_time;
  long click_time;
  char name;
};

static struct Button Button_L;
static struct Button Button_R;
// -----------------------------------


// -------- button callbacks ---------
void buttonHoldNotificationCallback(uint8_t n);
void buttonHoldReleasedCallback(uint8_t n);
void buttonClickCallback(uint8_t n);
void buttonReleaseCallback(uint8_t n);
// -----------------------------------

volatile bool button_L_changed = false;
volatile bool button_R_changed = false;

// ----------- buttons isr -----------
void IRAM_ATTR button_L_isr() {
  button_L_changed = true;
}

void IRAM_ATTR button_R_isr() {
  button_R_changed = true;
}
// -----------------------------------

// ------------ other ------------
long last_print = 0;
int counter = 0;
bool BATTERY_AA_MODE = true;
// -------------------------------

uint8_t BUTTON1_PREV = 0;
uint8_t BUTTON2_PREV = 0;


void setup() {
  
  Serial.begin(9600);

  print_wakeup_reason();

  initNeopixels();
  initNeoAnimations();

  setNeoAnim(&neo_animation_home, NEO_ANIM_FUNKY, NEO_ANIM_ALERT);
  setNeoAnimColours(&neo_animation_home, NEO_LAVENDER, NEO_GREEN);
  setNeoAnimSpeed(&neo_animation_home, 300);
  startNeoAnim(&neo_animation_home);

  long start_del = millis();
  while(millis()-start_del < 300) {
    updateNeoAnimation();
  }

  initSensors();
  initProximity();
  initIMU();
  initButtons();
  initSound();

  playSound(SOUND_FLUTTER_JOY);

  int battery_val = analogRead(BATT_SIG_PIN);
  if(battery_val > 0) {
    Serial << battery_val << " BATTERY_AA_MODE enabled" << endl;
    BATTERY_AA_MODE = true;
  } else {
    Serial << battery_val << " BATTERY_AA_MODE disabled" << endl;
    BATTERY_AA_MODE = false;
  }

  if(BATTERY_AA_MODE == true) {

    initServos(1);

    // battery catch up
    setNeoAnimColours(&neo_animation_home, NEO_ORANGE, NEO_SKY_BLUE);
    start_del = millis();
    while(millis()-start_del < 1000) {
      updateNeoAnimation();
    }

    initServos(2);

    // battery catch up
    setNeoAnimColours(&neo_animation_home, NEO_CANARY_YELLOW, NEO_GREEN);
    start_del = millis();
    while(millis()-start_del < 1000) {
      updateNeoAnimation();
    }

    initialised_servos = true; // remember to set the flag when initialising motors separately

  } else {

    initServos(0);

  }

  initServoAnimations();

  playSound(SOUND_ALERT_STARTUP);

  changeState(STATE1); // this is where you can set the initial state

  Serial << "Ready" << endl;

}

void loop() {
  
  updateStateMachine();
  updateButtons();

  console();

}


void console() {

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '1':
        changeState(STATE1);
      break;
      case '2':
        changeState(STATE2);
      break;
      case '3':
        changeState(STATE3);
      break;
      case '4':
        changeState(STATE4);
      break;
      case '5':
        changeState(STATE5);
      break;
      case '6':
        changeState(STATE6);
      break;
      case '7':
        changeState(STATE7);
      break;
      case '8':
        changeState(STATE8);
      break;

      case 'a':
        decrementState();
      break;
      case 'd':
        incrementState();
      break;

      case 'h':
        Serial << "1-8: states" << endl;
        Serial << "a: r button" << endl;
        Serial << "d: l button" << endl;
        Serial << "h: help" << endl;
      break;
    }
  }

}

