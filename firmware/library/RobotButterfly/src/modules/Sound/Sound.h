#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include "pitches.h"

void initSound();
void updateSound();
void Task_SOUND_code(void * pvParameters);
void setSoundTaskPriority(uint8_t p);

void playSimpleTone(int freq, int duration);
void playNoTone();

void playSound(uint8_t id);
void soundDoneCallback(uint8_t id);
typedef void (*SoundCallback)(uint8_t id);
extern SoundCallback onSoundDoneCallback;

static uint8_t CURRENT_SOUND;

enum soundID {
  SOUND_NONE,

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

// ---------- rtos ------------
static TaskHandle_t Task_SOUND;
static SemaphoreHandle_t Mutex_SOUND;
static long last_sound_rtos_print;
// -------------------------------

#endif