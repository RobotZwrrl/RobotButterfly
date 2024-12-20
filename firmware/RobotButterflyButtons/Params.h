#define DEBUG_BUTTONS            true
#define DEBOUNCE_TIME            250    // ms
#define ACCIDENTAL_CLICK_TIME    200    // ms
#define BUTTON_HOLD_DURATION         1000   // hold for this long for a long press in ms
#define BUTTON_HOLD_NOTIF_DURATION      2000   // timeout long press after this much ms
#define MAX_CLICK_TIME           700    // amount of time between press and release to qualify as a click (in ms)
#define BUTTON_BOTH_RELEASE_LOCKOUT 1000 // amount of time to wait after both hold has been released to register another button hold
// ------------

#define LONG_DOUBLE_CHECK        200    // amount of time to wait after the long hold starts to check if both buttons are pressed in the long hold (in ms)
#define DOUBLE_CALLBACK_TIMEOUT  MAX_CLICK_TIME+100    // amount of time to wait for checking if both buttons were pressed / released before calling the callback
#define DOUBLE_PRESS_FLAG_CLEARANCE 100 // amount of time for the double press flag to clear
