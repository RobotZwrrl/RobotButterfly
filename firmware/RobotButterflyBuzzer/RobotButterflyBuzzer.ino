/*
 * Robot Butterfly Buzzer
 * -------------------------
 * Jingles and sounds on the buzzer
 * 
 * Board: ESP32 Dev Module
 * CPU speed: 80 MHz
 * Upload speed: 115200 baud
 * esp boards package: v 3.0.2
 *
 * 21 Mar 2025 - Erin RobotZwrrl for Robot Missions Inc
 * http://robotzwrrl.xyz/robot-butterfly
 * http://robotmissions.org
 */

#include "Board.h"
#include "Params.h"
#include <Streaming.h>
#include "pitches.h"

// ------- function definitions for reference --------
void playStartup();
void playAlert();
void playSleep();
void playSneeze();
void playMissionComplete();
void playMoonlightMode();
void playChirpy();
void playWiFiConnect();
void playWiFiDisconnect();
void playMQTTSend();
void playMQTTReceive();
void playFlowerSmell();
void playRoseSmell();
void playWildflowerSmell();
void playLavenderSmell();
void playDaisySmell();
void playSunflowerSmell();
void playFlutterMemory();
void playFlutterJoy();
void playFlutterCalm();
void playFlutterSurprise();
void playFlutterConfused();
void playFlutterSleepy();
void playFlutterGrateful();
// -----------------------------------



void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initBuzzer();

  Serial << "Ready" << endl;
}


void loop() {
  
  // if(millis()-last_print >= 500) {
  //   Serial << millis() << " hi " << xPortGetCoreID() << endl;
  //   last_print = millis();
  // }

  console();

}


void console() {

  if(Serial.available()) {
    char c = Serial.read();
    switch(c) {
      case '1':
        Serial << "playStartup" << endl;
        playStartup();
        Serial << "done" << endl;
      break;
      case '2':
        Serial << "playAlert" << endl;
        playAlert();
        Serial << "done" << endl;
      break;
      case '3':
        Serial << "playSleep" << endl;
        playSleep();
        Serial << "done" << endl;
      break;
      case '4':
        Serial << "playSneeze" << endl;
        playSneeze();
        Serial << "done" << endl;
      break;
      case '5':
        Serial << "playMissionComplete" << endl;
        playMissionComplete();
        Serial << "done" << endl;
      break;
      case '6':
        Serial << "playMoonlightMode" << endl;
        playMoonlightMode();
        Serial << "done" << endl;
      break;
      case '7':
        Serial << "playChirpy" << endl;
        playChirpy();
        Serial << "done" << endl;
      break;
      case '8':
        Serial << "playWiFiConnect" << endl;
        playWiFiConnect();
        Serial << "done" << endl;
      break;
      case '9':
        Serial << "playWiFiDisconnect" << endl;
        playWiFiDisconnect();
        Serial << "done" << endl;
      break;
        
      case 'q':
        Serial << "playMQTTSend" << endl;
        playMQTTSend();
        Serial << "done" << endl;
      break;
      case 'w':
        Serial << "playMQTTReceive" << endl;
        playMQTTReceive();
        Serial << "done" << endl;
      break;
      case 'e':
        Serial << "playFlowerSmell" << endl;
        playFlowerSmell();
        Serial << "done" << endl;
      break;
      case 'r':
        Serial << "playRoseSmell" << endl;
        playRoseSmell();
        Serial << "done" << endl;
      break;
      case 't':
        Serial << "playWildflowerSmell" << endl;
        playWildflowerSmell();
        Serial << "done" << endl;
      break;
      case 'y':
        Serial << "playLavenderSmell" << endl;
        playLavenderSmell();
        Serial << "done" << endl;
      break;
      case 'u':
        Serial << "playDaisySmell" << endl;
        playDaisySmell();
        Serial << "done" << endl;
      break;
      case 'i':
        Serial << "playSunflowerSmell" << endl;
        playSunflowerSmell();
        Serial << "done" << endl;
      break;
      case 'o':
        Serial << "playFlutterMemory" << endl;
        playFlutterMemory();
        Serial << "done" << endl;
      break;
      case 'p':
        Serial << "playFlutterJoy" << endl;
        playFlutterJoy();
        Serial << "done" << endl;
      break;

      case 'a':
        Serial << "playFlutterCalm" << endl;
        playFlutterCalm();
        Serial << "done" << endl;
      break;
      case 's':
        Serial << "playFlutterSurprise" << endl;
        playFlutterSurprise();
        Serial << "done" << endl;
      break;
      case 'd':
        Serial << "playFlutterConfused" << endl;
        playFlutterConfused();
        Serial << "done" << endl;
      break;
      case 'f':
        Serial << "playFlutterSleepy" << endl;
        playFlutterSleepy();
        Serial << "done" << endl;
      break;
      case 'g':
        Serial << "playFlutterGrateful" << endl;
        playFlutterGrateful();
        Serial << "done" << endl;
      break;

      case 'h':
        Serial << "1: playStartup" << endl;
        Serial << "2: playAlert" << endl;
        Serial << "3: playSleep" << endl;
        Serial << "4: playSneeze" << endl;
        Serial << "5: playMissionComplete" << endl;
        Serial << "6: playMoonlightMode" << endl;
        Serial << "7: playChirpy" << endl;
        Serial << "8: playWiFiConnect" << endl;
        Serial << "9: playWiFiDisconnect" << endl;
        Serial << "q: playMQTTSend" << endl;
        Serial << "w: playMQTTReceive" << endl;
        Serial << "e: playFlowerSmell" << endl;
        Serial << "r: playRoseSmell" << endl;
        Serial << "t: playWildflowerSmell" << endl;
        Serial << "y: playLavenderSmell" << endl;
        Serial << "u: playDaisySmell" << endl;
        Serial << "i: playSunflowerSmell" << endl;
        Serial << "o: playFlutterMemory" << endl;
        Serial << "p: playFlutterJoy" << endl;
        Serial << "a: playFlutterCalm" << endl;
        Serial << "s: playFlutterSurprise" << endl;
        Serial << "d: playFlutterConfused" << endl;
        Serial << "f: playFlutterSleepy" << endl;
        Serial << "g: playFlutterGrateful" << endl;
      break;
    }
  }

}

