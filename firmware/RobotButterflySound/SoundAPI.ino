void soundDoneCallback(uint8_t id) {
  if(DEBUG_BUZZER) Serial << "done sound (" << id << ")" << endl;
}


void playSound(uint8_t id) {
  bool play = false;
  switch (id) {
    case SOUND_ALERT_STARTUP:
      playStartup();
      play = true;
    break;
    case SOUND_ALERT_ALERT:
      playAlert();
      play = true;
    break;
    case SOUND_ALERT_SLEEP:
      playSleep();
      play = true;
    break;
    case SOUND_ALERT_SNEEZE:
      playSneeze();
      play = true;
    break;
    case SOUND_ALERT_MISSION_COMPLETE:
      playMissionComplete();
      play = true;
    break;
    case SOUND_ALERT_MOONLIGHT_MODE:
      playMoonlightMode();
      play = true;
    break;
    case SOUND_ALERT_CHIRPY:
      playChirpy();
      play = true;
    break;
    case SOUND_ALERT_WIFI_CONNECT:
      playWiFiConnect();
      play = true;
    break;
    case SOUND_ALERT_WIFI_DISCONNECT:
      playWiFiDisconnect();
      play = true;
    break;
    case SOUND_ALERT_MQTT_SEND:
      playMQTTSend();
      play = true;
    break;
    case SOUND_ALERT_MQTT_RECEIVE:
      playMQTTReceive();
      play = true;
    break;
    case SOUND_SMELL_FLOWER:
      playFlowerSmell();
      play = true;
    break;
    case SOUND_SMELL_ROSE:
      playRoseSmell();
      play = true;
    break;
    case SOUND_SMELL_WILDFLOWER:
      playWildflowerSmell();
      play = true;
    break;
    case SOUND_SMELL_LAVENDER:
      playLavenderSmell();
      play = true;
    break;
    case SOUND_SMELL_DAISY:
      playDaisySmell();
      play = true;
    break;
    case SOUND_SMELL_SUNFLOWER:
      playSunflowerSmell();
      play = true;
    break;
    case SOUND_FLUTTER_MEMORY:
      playFlutterMemory();
      play = true;
    break;
    case SOUND_FLUTTER_JOY:
      playFlutterJoy();
      play = true;
    break;
    case SOUND_FLUTTER_CALM:
      playFlutterCalm();
      play = true;
    break;
    case SOUND_FLUTTER_SURPRISE:
      playFlutterSurprise();
      play = true;
    break;
    case SOUND_FLUTTER_CONFUSED:
      playFlutterConfused();
      play = true;
    break;
    case SOUND_FLUTTER_SLEEPY:
      playFlutterSleepy();
      play = true;
    break;
    case SOUND_FLUTTER_GRATEFUL:
      playFlutterGrateful();
      play = true;
    break;
    default:
    break;
  }

  if(play) {
    if(onSoundDoneCallback) onSoundDoneCallback(id);
  }

}


