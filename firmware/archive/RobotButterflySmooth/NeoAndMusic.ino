
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


void playMelody() {

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }

}


void neoUpdate() {
  if(millis()-last_sparkle >= 100) {
    for(int i=0; i<strip.numPixels(); i++) {
      patternPurplePink(i);
    }
    neoSparkle();
    strip.show();
    last_sparkle = millis();
  }
}


void patternPurplePink(uint8_t i) {
  if(i%2 == 0) {
    strip.setPixelColor(i, strip.Color(170, 20, 130));
  } else {
    strip.setPixelColor(i, strip.Color(30, 60, 220));
  }
}

void neoSparkle() {
  int r1 = random(0, 8+1);
  strip.setPixelColor(r1, strip.Color(255, 240, 0));
  int r2 = random(0, 8+1);
  strip.setPixelColor(r2, strip.Color(255, 240, 0));
}


