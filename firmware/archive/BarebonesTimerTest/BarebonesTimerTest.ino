// this_apb_freq = 80000000

#include <Streaming.h>
#include "soc/rtc.h"
uint32_t this_apb_freq = rtc_clk_apb_freq_get();

//#define TRANSITION_FRAME_TIME    40000     // 5 seconds
//#define TRANSITION_FRAME_TIME    32000     // 4 seconds
//#define TRANSITION_FRAME_TIME    24000     // 3 seconds
#define TRANSITION_FRAME_TIME    16000       // 2 seconds
//#define TRANSITION_FRAME_TIME    8000      // 1 seconds
#define STATE_TIMER_PRESCALER    8000

volatile bool enter_state = false;

long t_transition = 0;
long t_enter = 0;
long t_delta = 0;

hw_timer_t *timer1_cfg = NULL;
void IRAM_ATTR Timer1_ISR() {
  enter_state = true;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  if(enter_state) {
    t_enter = millis();
    t_delta = t_enter - t_transition;
    Serial << "enter: " << t_enter << endl;
    Serial << "delta: " << t_delta << endl;
    enter_state = false;

    if(timer1_cfg != NULL) {
      Serial << "timer deleted" << endl;
      timerEnd(timer1_cfg);
    }
  }

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case 'a':
        Serial << c << endl;
        transitionState();
      break;
      case 'h':
        Serial << "hello" << endl;
        Serial << this_apb_freq << endl;
      break;
    }
  }

}


void transitionState() {
  t_transition = millis();
  Serial << "transition: " << t_transition << endl;

  timer1_cfg = timerBegin(STATE_TIMER_PRESCALER);
  //timer1_cfg = timerBegin(80000);
  timerAttachInterrupt(timer1_cfg, &Timer1_ISR);
  // params: timer, tick count, auto-reload (false to run once), reload count (0 = infinite)
  //timerAlarm(timer1_cfg, 5000, false, 0);
  timerAlarm(timer1_cfg, TRANSITION_FRAME_TIME, false, 0);
  timerStart(timer1_cfg);
}



