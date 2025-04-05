/*
 * Robot Butterfly Sound
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


// ----------- buzzer api ------------
void playSound(uint8_t id);
void soundDoneCallback(uint8_t id);
// -----------------------------------


// ------------ sound id -------------
enum soundID {
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
// -----------------------------------


// ------------- other ---------------
long last_print = 0;
// -----------------------------------


void setup() {
  Serial.begin(9600);

  print_wakeup_reason();

  initBuzzer();

  Serial << "Ready" << endl;
}


void loop() {

  console();

}


void console() {
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case '1':
        Serial << "playStartup" << endl;
        playSound(SOUND_ALERT_STARTUP);
      break;
      case '2':
        Serial << "playAlert" << endl;
        playSound(SOUND_ALERT_ALERT);
      break;
      case '3':
        Serial << "playSleep" << endl;
        playSound(SOUND_ALERT_SLEEP);
      break;
      case '4':
        Serial << "playSneeze" << endl;
        playSound(SOUND_ALERT_SNEEZE);
      break;
      case '5':
        Serial << "playMissionComplete" << endl;
        playSound(SOUND_ALERT_MISSION_COMPLETE);
      break;
      case '6':
        Serial << "playMoonlightMode" << endl;
        playSound(SOUND_ALERT_MOONLIGHT_MODE);
      break;
      case '7':
        Serial << "playChirpy" << endl;
        playSound(SOUND_ALERT_CHIRPY);
      break;
      case '8':
        Serial << "playWiFiConnect" << endl;
        playSound(SOUND_ALERT_WIFI_CONNECT);
      break;
      case '9':
        Serial << "playWiFiDisconnect" << endl;
        playSound(SOUND_ALERT_WIFI_DISCONNECT);
      break;

      case 'q':
        Serial << "playMQTTSend" << endl;
        playSound(SOUND_ALERT_MQTT_SEND);
      break;
      case 'w':
        Serial << "playMQTTReceive" << endl;
        playSound(SOUND_ALERT_MQTT_RECEIVE);
      break;
      case 'e':
        Serial << "playFlowerSmell" << endl;
        playSound(SOUND_SMELL_FLOWER);
      break;
      case 'r':
        Serial << "playRoseSmell" << endl;
        playSound(SOUND_SMELL_ROSE);
      break;
      case 't':
        Serial << "playWildflowerSmell" << endl;
        playSound(SOUND_SMELL_WILDFLOWER);
      break;
      case 'y':
        Serial << "playLavenderSmell" << endl;
        playSound(SOUND_SMELL_LAVENDER);
      break;
      case 'u':
        Serial << "playDaisySmell" << endl;
        playSound(SOUND_SMELL_DAISY);
      break;
      case 'i':
        Serial << "playSunflowerSmell" << endl;
        playSound(SOUND_SMELL_SUNFLOWER);
      break;
      case 'o':
        Serial << "playFlutterMemory" << endl;
        playSound(SOUND_FLUTTER_MEMORY);
      break;
      case 'p':
        Serial << "playFlutterJoy" << endl;
        playSound(SOUND_FLUTTER_JOY);
      break;

      case 'a':
        Serial << "playFlutterCalm" << endl;
        playSound(SOUND_FLUTTER_CALM);
      break;
      case 's':
        Serial << "playFlutterSurprise" << endl;
        playSound(SOUND_FLUTTER_SURPRISE);
      break;
      case 'd':
        Serial << "playFlutterConfused" << endl;
        playSound(SOUND_FLUTTER_CONFUSED);
      break;
      case 'f':
        Serial << "playFlutterSleepy" << endl;
        playSound(SOUND_FLUTTER_SLEEPY);
      break;
      case 'g':
        Serial << "playFlutterGrateful" << endl;
        playSound(SOUND_FLUTTER_GRATEFUL);
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


