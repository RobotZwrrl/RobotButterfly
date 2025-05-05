#ifndef NEOANIMATION_H
#define NEOANIMATION_H

#include <Arduino.h>
#include <Streaming.h>
#include "Board.h"
#include "Params.h"
#include "ParamsRTOS.h"
#include <Adafruit_NeoPixel.h>


// hi!
struct NeoAnimation;

// ----------- neoanim callbacks -----------
typedef void (*NeoCallback)(struct NeoAnimation *a); // neo animation callback type
extern NeoCallback onNeoAnimDoneCallback;
extern NeoCallback onNeoAnimLoopCallback;
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

extern NeoAnimation neo_animation_home;
extern NeoAnimation neo_animation_alert;
// -----------------------------------

// ----------- neopixel colours -----------
// go with 400 kHz as 800 kHz would leave pixel colour ghosts
static Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ400);

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

static String pastelNames[NUM_PALETTE_COLOURS] = {
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
static uint16_t pastelColours[NUM_PALETTE_COLOURS][3] = {
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

static uint32_t colourPalette[NUM_PALETTE_COLOURS];
// -----------------------------------

// ------- neoanim functions ---------
void initNeopixels();
void initNeoAnimations();
void initNeoColours();
void updateNeoAnimation();
void Task_NEOANIM_code(void * pvParameters);
void setNeoAnimationTaskPriority(uint8_t p);

void setNeoAnim(struct NeoAnimation *a, uint8_t n, uint8_t t);
void setNeoAnimColours(struct NeoAnimation *a, uint8_t c1, uint8_t c2);
void setNeoAnimDuration(struct NeoAnimation *a, long duration);
void setNeoAnimRepeats(struct NeoAnimation *a, int r);
void setNeoAnimSpeed(struct NeoAnimation *a, uint16_t del);
void startNeoAnim(struct NeoAnimation *a);
void stopNeoAnim(struct NeoAnimation *a);
void setNeoAnimDir(struct NeoAnimation *a, bool dir);
void setNeoAnimRangeVal(struct NeoAnimation *a, int val);
void setNeoAnimRainbowSteps(struct NeoAnimation *a, int val);
void setNeoAnimBreatheSteps(struct NeoAnimation *a, int val);
void setNeoAnimBreatheMaxBrightness(struct NeoAnimation *a, int val);
void setNeoAnimAmbiance(struct NeoAnimation *a, int id);
void setNeoAnimUno(struct NeoAnimation *a, int uno);
void setNeoAnimDuo(struct NeoAnimation *a, int duo);
// -----------------------------------

// ---------- rtos ------------
static TaskHandle_t Task_NEOANIM;
static SemaphoreHandle_t Mutex_NEOANIM;
static long last_neoanim_rtos_print;
// -------------------------------

#endif