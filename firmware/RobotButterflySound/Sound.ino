void updateSound() {

  if(CURRENT_SOUND == SOUND_NONE) return;

  bool play = false;
  switch(CURRENT_SOUND) {
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
    if(onSoundDoneCallback) onSoundDoneCallback(CURRENT_SOUND);
    CURRENT_SOUND = SOUND_NONE;
  }

}


void playStartup() {
  int melody[] = {
    NOTE_E5, NOTE_G5, NOTE_B5, NOTE_E6,
    NOTE_DS6, NOTE_B5, NOTE_G5,
    NOTE_A5, NOTE_FS5, NOTE_D6
  };

  int noteDurations[] = {
    8, 8, 8, 4,
    8, 8, 4,
    8, 8, 2
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }
}

void playAlert() {
  int melody[] = {
    NOTE_E5, NOTE_G5, NOTE_B5, NOTE_E6
  };

  int noteDurations[] = {
    16, 16, 16, 2
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }
}

void playSleep() {
  int melody[] = {
    NOTE_D6, NOTE_B5, NOTE_A5, NOTE_E5, NOTE_B4
  };

  int noteDurations[] = {
    4, 4, 4, 4, 2
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.3);
    playNoTone();
  }
}

void playSneeze() {
  // build-up — a rising wobble, like a pre-sneeze sniffle
  int buildup[] = { NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5 };
  for (int i = 0; i < 4; i++) {
    playSimpleTone(buildup[i], 50);
    delay(60);
    playNoTone();
  }

  delay(80); // pause... uh oh...

  // sneeze burst! AACHOOOO
  playSimpleTone(NOTE_A3, 200);
  delay(200);
  playSimpleTone(NOTE_E3, 150);
  delay(150);
  playNoTone();

  // post-sneeze silence
  delay(250);
}

void playMissionComplete() {
  int melody[] = {
    NOTE_G4, NOTE_B4, NOTE_D5,
    NOTE_E5, NOTE_G5, NOTE_E5,
    NOTE_C5
  };

  int noteDurations[] = {
    8, 8, 8,
    8, 8, 8,
    2  // Big strong finish!
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  delay(200);
}

void playMoonlightMode() {
  int melody[] = {
    NOTE_A4, NOTE_CS5, NOTE_E5,
    NOTE_B4, NOTE_D5, NOTE_FS5,
    NOTE_E5, NOTE_CS5, NOTE_A4
  };

  int noteDurations[] = {
    6, 6, 4,
    6, 6, 4,
    4, 6, 2  // Ends slow and soft
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.4);
    playNoTone();
  }

}

void playChirpy() {
  int melody[] = {
    NOTE_C5, NOTE_E5, NOTE_G5,
    NOTE_E5, NOTE_G5, NOTE_C6,
    NOTE_B5, NOTE_G5, NOTE_A5,
    NOTE_FS5, NOTE_G5, NOTE_E5,
    NOTE_D5, NOTE_E5, NOTE_C5
  };

  int noteDurations[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 4,
    8, 8, 4,
    8, 8, 2
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int duration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], duration);
    delay(duration * 1.2);
    playNoTone();
  }

  delay(150);
}

// ------------------------------------
// ------------------------------------

void playWiFiConnect() {
  int melody[] = {
    NOTE_D4, NOTE_G4, NOTE_B4,
    NOTE_A4, NOTE_D5
  };

  int noteDurations[] = {
    8, 8, 8,
    8, 2  // final note is long and warm
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  delay(100);
}

void playWiFiDisconnect() {
  int melody[] = {
    NOTE_D5, NOTE_A4, NOTE_B4, NOTE_G4, NOTE_D4
  };

  int noteDurations[] = {
    8, 8, 8, 8, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  delay(100);
}

void playMQTTSend() {
  int melody[] = {
    NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_CS5
  };

  int noteDurations[] = {
    16, 16, 8, 16
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  delay(80);
}

void playMQTTReceive() {
  int melody[] = {
    NOTE_E5, NOTE_CS5, NOTE_A4, NOTE_B3
  };

  int noteDurations[] = {
    16, 16, 8, 16
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  delay(80);
}

// ------------------------------------
// ------------------------------------

void playFlowerSmell() {
  int melody[] = {
    NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_E5
  };

  int noteDurations[] = {
    8, 8, 8, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

  for (int i = 0; i < 4; i++) {
    playSimpleTone((i % 2 == 0) ? NOTE_E5 : NOTE_DS5, 60); // Alternate tones
    delay(70);
    playNoTone();
  }
}

void playRoseSmell() {
  int melody[] = {
    NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_E5,
    NOTE_A4, NOTE_B4, NOTE_E5, NOTE_D5
  };

  int noteDurations[] = {
    6, 6, 6, 4,
    8, 8, 6, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.3);
    playNoTone();
  }

}

void playWildflowerSmell() {
  int melody[] = {
    NOTE_G4, NOTE_A4, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_G5, NOTE_E5
  };

  int noteDurations[] = {
    8, 8, 4, 8,
    8, 8, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.2);
    playNoTone();
  }

}

void playLavenderSmell() {
  int melody[] = {
    NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_A5,
    NOTE_CS5, NOTE_E5
  };

  int noteDurations[] = {
    6, 6, 6, 4,
    8, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.3); // slightly slower, smoother
    playNoTone();
  }

}

void playDaisySmell() {
  int melody[] = {
    NOTE_G5, NOTE_B5, NOTE_A5, NOTE_C6,
    NOTE_D6, NOTE_B5
  };

  int noteDurations[] = {
    8, 8, 8, 8,
    4, 4
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.1);
    playNoTone();
  }
}

void playSunflowerSmell() {
  int melody[] = {
    NOTE_E4, NOTE_G4, NOTE_B4,
    NOTE_E5, NOTE_D5, NOTE_G4,
    NOTE_B4, NOTE_CS5, NOTE_E5, NOTE_G5
  };

  int noteDurations[] = {
    6, 6, 6,
    4, 8, 4,
    8, 8, 6, 2
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    int noteDuration = 1000 / noteDurations[i];
    playSimpleTone(melody[i], noteDuration);
    delay(noteDuration * 1.3);
    playNoTone();
  }

}

// ------------------------------------
// ------------------------------------

void playFlutterMemory() {
  for (int i = 0; i < 3; i++) {
    playSimpleTone((i % 2 == 0) ? NOTE_E6 : NOTE_D6, 50);
    delay(60);
    playNoTone();
  }
  delay(100);
}

void playFlutterJoy() {
  for (int i = 0; i < 4; i++) {
    playSimpleTone((i % 2 == 0) ? NOTE_G5 : NOTE_A5, 40);
    delay(50);
    playNoTone();
  }
  delay(80);
}

void playFlutterCalm() {
  int notes[] = { NOTE_E5, NOTE_D5, NOTE_CS5 };
  for (int i = 0; i < 3; i++) {
    playSimpleTone(notes[i], 100);
    delay(120);
    playNoTone();
  }
  delay(150);
}

void playFlutterSurprise() {
  int notes[] = { NOTE_B4, NOTE_E5, NOTE_B4 };
  for (int i = 0; i < 3; i++) {
    playSimpleTone(notes[i], 60);
    delay(70);
    playNoTone();
  }
  delay(100);
}

void playFlutterConfused() {
  int notes[] = { NOTE_D5, NOTE_CS5, NOTE_D5, NOTE_CS5, NOTE_E4 };
  int durations[] = { 50, 50, 50, 50, 150 };

  for (int i = 0; i < 5; i++) {
    playSimpleTone(notes[i], durations[i]);
    delay(durations[i] + 20);
    playNoTone();
  }

  delay(100);
}

void playFlutterSleepy() {
  int notes[] = { NOTE_E5, NOTE_CS5, NOTE_A4 };
  for (int i = 0; i < 3; i++) {
    playSimpleTone(notes[i], 120);
    delay(140);
    playNoTone();
  }

  delay(200);
}

void playFlutterGrateful() {
  int notes[] = { NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_E5 };
  int durations[] = { 100, 100, 100, 150 };

  for (int i = 0; i < 4; i++) {
    playSimpleTone(notes[i], durations[i]);
    delay(durations[i] + 20);
    playNoTone();
  }

  playSimpleTone(NOTE_D6, 50);
  delay(60);
  playNoTone();

  delay(150);
}

// ------------------------------------
// ------------------------------------

void playSimpleTone(int freq, int duration) {
  int pwmVal = 127; // Half duty cycle
  int delayMicros = 1000000 / freq / 2;

  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(delayMicros);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(delayMicros);
  }
}

void playNoTone() {
  digitalWrite(BUZZER_PIN, LOW);
}

void initSound() {
  onSoundDoneCallback = soundDoneCallback;
  pinMode(BUZZER_PIN, OUTPUT);
}


