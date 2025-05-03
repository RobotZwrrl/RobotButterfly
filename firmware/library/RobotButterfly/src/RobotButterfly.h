#ifndef ROBOTBUTTERFLY_H
#define ROBOTBUTTERFLY_H

#include <Arduino.h>
#include <Streaming.h>

// library dependencies:
// Streaming - v5 https://github.com/janelia-arduino/Streaming
// movingAvg - v2.3.2 https://github.com/JChristensen/movingAvg
// MPU6050.h - v1.4.1 https://github.com/ElectronicCats/mpu6050
// Adafruit Neopixel - v.1.12.4 https://github.com/adafruit/Adafruit_NeoPixel
// ESP32Servo - v3.0.6 https://github.com/madhephaestus/ESP32Servo 
// ServoEasing - v3.4.0 https://github.com/ArminJo/ServoEasing
// DHT.h - v1.4.6 https://github.com/adafruit/DHT-sensor-library & 1.1.15 https://github.com/adafruit/Adafruit_Sensor 
// HCSR04.h - v1.1.3 https://github.com/d03n3rfr1tz3/HC-SR04 


#include "modules/Buttons/Buttons.h"
#include "modules/Sound/Sound.h"
#include "modules/IMU/IMU.h"
#include "modules/NeoAnimation/NeoAnimation.h"
#include "modules/ServoAnimation/ServoAnimation.h"
#include "modules/Sensors/Sensors.h"
#include "modules/Proximity/Proximity.h"
//#include "modules/StateMachine.h"

#define ROBOT_BUTTERFLY_LIBRARY_VERSION "0.0.1"

class RobotButterfly {
public:
    RobotButterfly();
    void init();
    void update();

    typedef void (*StateSetup)();
    typedef void (*StateLoop)();
    
    static void addState(uint8_t id, StateSetup setup_fn, StateLoop loop_fn);
    static void changeState(uint8_t n);
    static void incrementState();
    static void decrementState();
    static void printStateHeartbeat(uint8_t id);

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

    static struct State state1;
    static struct State state2;
    static struct State state3;
    static struct State state4;
    static struct State state5;
    static struct State state6;
    static struct State state7;
    static struct State state8;

    static hw_timer_t *timer_state_cfg;

    // -- button callbacks --
    static void buttonHoldNotificationCallback(uint8_t n);
    static void buttonHoldReleasedCallback(uint8_t n);
    static void buttonClickCallback(uint8_t n);
    static void buttonReleaseCallback(uint8_t n);
    // TODO: add user callbacks
    // --

    // -- sound callbacks --
    static void soundDoneCallback(uint8_t id);
    // TODO: add user callbacks
    // --

    // -- imu callbacks --
    static void imuStateChangeCallback(uint8_t s);
    static void imuOrientationChangeCallback(uint8_t o);
    static void imuPoseChangeCallback(uint8_t p);
    static void imuEventDetectedCallback(uint8_t e);
    // TODO: add user callbacks
    // --

    // -- neoanimation callbacks --
    static void neoAnimDoneCallback(struct NeoAnimation *a);
    static void neoAnimLoopCallback(struct NeoAnimation *a);
    // TODO: add user callbacks
    // --

    // -- servoanimation callbacks --
    static void servoAnimDoneCallback(struct ServoAnimation *a);
    static void servoAnimLoopCallback(struct ServoAnimation *a);
    // TODO: add user callbacks
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
    // TODO: add user callbacks
    // --

    // -- proximity callbacks --
    static void proximityTriggerCallback(struct Proximity *p);
    // TODO: add user callbacks
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

