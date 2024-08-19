#define DEBUG_BUTTONS true
#define DEBOUNCE_TIME 250 // ms
#define LONG_BUTTON_HOLD 1000 // hold for this long for a long press in ms
#define LONG_BUTTON_TIMEOUT 2000 // timout long press after this much ms


#define DEBUG_PATT       true
#define PATT_MAX         20      // max number of animation frames

#define DEBUG_NP         true    // Task_NP_code()
#define WARN_NP          true    // primarily for unhandled mutex exceptions
#define STARTUP_WAIT_NP  0       // wait for the neopixels to warm up
#define STACK_NP         2000   // Task_NP stack size

#define PATT_MIN_DWELL   80      // min dwell time in ms