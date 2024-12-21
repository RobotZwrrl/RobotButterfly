#define DEBUG_BUTTONS                  false   // debug print statements
#define DEBOUNCE_TIME                    250   // (isr) time for the signal to settle
#define ACCIDENTAL_CLICK_TIME            200   // (isr) prevent bouncing double clicks
#define BUTTON_HOLD_DURATION            1000   // hold for this long for a long press in ms
#define BUTTON_HOLD_NOTIF_DURATION      1200   // timeout long press after this much ms
#define MAX_CLICK_TIME                   700   // amount of time between press and release to qualify as a click (in ms)
#define BUTTON_BOTH_RELEASE_LOCKOUT     1000   // amount of time to wait after both hold has been released to register another button hold
#define NEXT_INDIVIDUAL_BUTTON_HOLD      200   // amount of time after a both button hold til an individual button hold is registered

// --------- RTOS ----------
#define STACK_BUTTONS         2048   // Task_BUTTONS stack size
#define TASK_CORE_BUTTONS        0
#define TASK_FREQ_BUTTONS        1
#define PRIORITY_BUTTONS         1