#define DEBUG_STATEMACHINE       true
#define MAX_STATE                3
#define TRANSITION_FRAME_TIME    5000   // how long to show the neopixels when changing states (in ms)
#define STATE_LOOP_PRINT         2000   // how long to print this every iteration of the state loop (in ms)

#define DEBUG_BUTTONS            true
#define DEBOUNCE_TIME            250    // ms
#define ACCIDENTAL_CLICK_TIME    200    // ms
#define LONG_BUTTON_HOLD         1000   // hold for this long for a long press in ms
#define LONG_BUTTON_TIMEOUT      2000   // timeout long press after this much ms
#define LONG_DOUBLE_CHECK        200    // amount of time to wait after the long hold starts to check if both buttons are pressed in the long hold (in ms)
#define MAX_CLICK_TIME           500    // amount of time between press and release to qualify as a click (in ms)