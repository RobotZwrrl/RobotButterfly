#ifndef SERVOANIMATION_H
#define SERVOANIMATION_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include <ESP32Servo.h>
//#include "ServoEasing.hpp" // this is included in the .cpp file


// hi!
struct ServoAnimation;
class ServoEasing;

// ----------- callbacks -----------
typedef void (*ServoAnimCallback)(struct ServoAnimation *a);
extern ServoAnimCallback onServoAnimDoneCallback;
extern ServoAnimCallback onServoAnimLoopCallback;
// -----------------------------------

// ----------- servo motors -----------
extern ServoEasing s1;
extern ServoEasing s2;

struct ServoMotor {
  ServoEasing *motor;
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

static struct ServoMotor wing_left;
static struct ServoMotor wing_right;

static volatile bool initialised_servos;
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
  SERVO_ANIM_FLUTTER_WINGS_BOTH_DOWN,
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

extern ServoAnimation servo_animation_home;
extern ServoAnimation servo_animation_alert;
// -----------------------------------

// ------- servoanim functions -------
void initServos(uint8_t mode);
void initServoAnimations();
void updateServoAnimation();
void Task_SERVOANIM_code(void * pvParameters);
void setServoAnimationTaskPriority(uint8_t p);

void setServoAnim(struct ServoAnimation *a, uint8_t n, uint8_t t);
void setServoAnimDuration(struct ServoAnimation *a, long duration);
void setServoAnimRepeats(struct ServoAnimation *a, int r);
void setServoAnimSpeed(struct ServoAnimation *a, uint16_t del);
void startServoAnim(struct ServoAnimation *a);
void stopServoAnim(struct ServoAnimation *a);
void setServoAnimRange(struct ServoAnimation *a, int n);
void setServoAnimFlutterWings(struct ServoAnimation *a, int n);
void setServoAnimFlutterPos(struct ServoAnimation *a, int n);
void setServoAnimFlutterOffset(struct ServoAnimation *a, int n);
void setServoAnimRangeSpan(struct ServoAnimation *a, int n);
void setServoAnimRangeVal(struct ServoAnimation *a, int n);
void setServoAnimPositionLeft(struct ServoAnimation *a, int n);
void setServoAnimPositionRight(struct ServoAnimation *a, int n);
int getServoSafeValLeft(int n);
int getServoSafeValRight(int n);
// -----------------------------------

// ---------- rtos ------------
static TaskHandle_t Task_SERVOANIM;
static SemaphoreHandle_t Mutex_SERVOANIM;
static long last_servoanim_rtos_print;
// -------------------------------

#endif