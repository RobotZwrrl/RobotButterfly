#ifndef ROBOTBUTTERFLY_H
#define ROBOTBUTTERFLY_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"

// board:
// esp32 dev module
// esp32 v2.0.4

// library dependencies:
// Streaming - v5.0.0 https://github.com/janelia-arduino/Streaming
// movingAvg - v2.3.2 https://github.com/JChristensen/movingAvg
// MPU6050 - v1.4.3 https://github.com/ElectronicCats/mpu6050
// Adafruit Neopixel - v.1.12.4 https://github.com/adafruit/Adafruit_NeoPixel
// ESP32Servo - v3.0.6 https://github.com/madhephaestus/ESP32Servo 
// ServoEasing - v3.4.0 https://github.com/ArminJo/ServoEasing
// DHT - v1.4.6 https://github.com/adafruit/DHT-sensor-library
// Adafruit Unified Sensor - v1.1.15 https://github.com/adafruit/Adafruit_Sensor 
// HC-SR04 - v1.1.3 https://github.com/d03n3rfr1tz3/HC-SR04 

#include "modules/Buttons/Buttons.h"
#include "modules/Sound/Sound.h"
#include "modules/IMU/IMU.h"
#include "modules/NeoAnimation/NeoAnimation.h"
#include "modules/ServoAnimation/ServoAnimation.h"
#include "modules/Sensors/Sensors.h"
#include "modules/Proximity/Proximity.h"


// -- updates --
enum UpdateOptions {
    UPDATE_STATEMACHINE_ON,
    UPDATE_STATEMACHINE_OFF,
    UPDATE_BUTTONS_ON,
    UPDATE_BUTTONS_OFF,
    UPDATE_SOUND_ON,
    UPDATE_SOUND_OFF,
    UPDATE_IMU_ON,
    UPDATE_IMU_OFF,
    UPDATE_NEOANIM_ON,
    UPDATE_NEOANIM_OFF,
    UPDATE_SERVOANIM_ON,
    UPDATE_SERVOANIM_OFF,
    UPDATE_SENSORS_ON,
    UPDATE_SENSORS_OFF,
    UPDATE_PROXIMITY_ON,
    UPDATE_PROXIMITY_OFF
};
// --


class RobotButterfly {
public:
    RobotButterfly();
    void init(bool init_servos, bool state_machine);
    void update();
    void update(uint8_t update_statemachine, 
                uint8_t update_buttons, 
                uint8_t update_sound, 
                uint8_t update_imu, 
                uint8_t update_neoanim, 
                uint8_t update_servoanim, 
                uint8_t update_sensors, 
                uint8_t update_proximity);

    // -- state machine --
    typedef void (*StateSetup)();
    typedef void (*StateLoop)();
    
    static void addState(uint8_t id, StateSetup setup_fn, StateLoop loop_fn);
    static void changeState(uint8_t n);
    static void incrementState();
    static void decrementState();
    static void printStateHeartbeat(uint8_t id);

    static hw_timer_t *timer_state_cfg;

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

    struct State {
        bool enabled;
        uint8_t id;
        StateSetup setup_fn;
        StateLoop loop_fn;
        long t_enter;
        long t_delta;
        long t_transition;
        bool new_print;
        long last_state_print;
        long last_state_change;
    };

    static uint8_t CURRENT_STATE;
    static uint8_t PREV_STATE;
    static struct State *all_states[NUM_STATES];
    static bool CHANGE_STATES_CONTROL;
    static uint8_t MAX_STATES_COUNT;

    static struct State state1;
    static struct State state2;
    static struct State state3;
    static struct State state4;
    static struct State state5;
    static struct State state6;
    static struct State state7;
    static struct State state8;
    // --
    

    // -- button callbacks --
    static void buttonHoldNotificationCallback(uint8_t n);
    static void buttonHoldReleasedCallback(uint8_t n);
    static void buttonClickCallback(uint8_t n);
    static void buttonReleaseCallback(uint8_t n);
    
    static ButtonCallback onHoldNotificationCallback_client;
    static ButtonCallback onHoldReleasedCallback_client;
    static ButtonCallback onClickCallback_client;
    static ButtonCallback onReleaseCallback_client;
    // --

    // -- sound callbacks --
    static void soundDoneCallback(uint8_t id);

    static SoundCallback onSoundDoneCallback_client;
    // --

    // -- imu callbacks --
    static void imuStateChangeCallback(uint8_t s);
    static void imuOrientationChangeCallback(uint8_t o);
    static void imuPoseChangeCallback(uint8_t p);
    static void imuEventDetectedCallback(uint8_t e);
    
    static IMUCallback onStateChangeCallback_client;
    static IMUCallback onOrientationChangeCallback_client;
    static IMUCallback onPoseChangeCallback_client;
    static IMUCallback onEventDetectedCallback_client;
    // --

    // -- neoanimation callbacks --
    static void neoAnimDoneCallback(struct NeoAnimation *a);
    static void neoAnimLoopCallback(struct NeoAnimation *a);
    
    static NeoCallback onNeoAnimDoneCallback_client;
    static NeoCallback onNeoAnimLoopCallback_client;
    // --

    // -- servoanimation callbacks --
    static void servoAnimDoneCallback(struct ServoAnimation *a);
    static void servoAnimLoopCallback(struct ServoAnimation *a);
    
    static ServoAnimCallback onServoAnimDoneCallback_client;
    static ServoAnimCallback onServoAnimLoopCallback_client;
    // --

    // -- sensors callbacks --
    static void sensorLightChangeCallback(struct Sensor *s, bool trigger_dir);
    static void sensorLightAmbientChangeCallback(struct Sensor *s, int change);
    static void sensorSoundChangeCallback(struct Sensor *s, bool trigger_dir);
    static void sensorSoundAmbientChangeCallback(struct Sensor *s, int change);
    static void sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir);
    static void sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change);
    static void sensorHumidityChangeCallback(struct Sensor *s, bool trigger_dir);
    static void sensorHumidityAmbientChangeCallback(struct Sensor *s, int change);
    
    static SensorTriggerCallback onSensorLightChangeCallback_client;
    static SensorAmbientCallback onSensorLightAmbientChangeCallback_client;
    static SensorTriggerCallback onSensorSoundChangeCallback_client;
    static SensorAmbientCallback onSensorSoundAmbientChangeCallback_client;
    static SensorTriggerCallback onSensorTemperatureChangeCallback_client;
    static SensorAmbientCallback onSensorTemperatureAmbientChangeCallback_client;
    static SensorTriggerCallback onSensorHumidityChangeCallback_client;
    static SensorAmbientCallback onSensorHumidityAmbientChangeCallback_client;
    // --

    // -- proximity callbacks --
    static void proximityTriggerCallback(struct Proximity *p);
    
    static ProximityCallback onProximityTriggerCallback_client;
    // --


private:

    static void initStateMachine();
    static void updateStateMachine();
    static void transitionState();

};

extern volatile bool new_enter;
extern volatile bool new_update;
extern volatile bool enter_state;
extern volatile bool update_state;
extern volatile bool new_print;

#endif

