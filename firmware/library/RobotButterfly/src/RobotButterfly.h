#ifndef ROBOTBUTTERFLY_H
#define ROBOTBUTTERFLY_H

#include <Arduino.h>
#include <Streaming.h>

// library dependencies:
// streaming - https://github.com/janelia-arduino/Streaming
// movingAvg - https://github.com/JChristensen/movingAvg
// MPU6050.h - https://github.com/ElectronicCats/mpu6050

#include "modules/Buttons/Buttons.h"
#include "modules/Sound/Sound.h"
#include "modules/IMU/IMU.h"
//#include "modules/NeoAnimation.h"
//#include "modules/NeoColours.h"
//#include "modules/Proximity.h"
//#include "modules/Sensors.h"
//#include "modules/ServoAnimation.h"
//#include "modules/Sound.h"
//#include "modules/StateMachine.h"

class RobotButterfly {
public:
    RobotButterfly();
    
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

    void init();
    void update();

private:
    
};

#endif

