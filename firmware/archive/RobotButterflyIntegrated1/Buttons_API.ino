// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------

// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      tone(BUZZER_PIN, NOTE_F5, 500);
      noTone(BUZZER_PIN);
      delay(200);
    break;
    case BUTTON_LEFT:
      tone(BUZZER_PIN, NOTE_A5, 500);
      noTone(BUZZER_PIN);
    break;
    case BUTTON_RIGHT:
      tone(BUZZER_PIN, NOTE_A7, 500);
      noTone(BUZZER_PIN);
    break;
  }
}

// do an action here
void buttonHoldReleasedCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
      counter *= 2;
    break;
    case BUTTON_LEFT:
      counter += 10;
    break;
    case BUTTON_RIGHT:
      counter -= 10;
    break;
  }
  Serial << "count: " << counter << endl;
}

// do an action here
void buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_LEFT:
      playSimpleTone(NOTE_A5, 100);
      playNoTone();
      incrementState();
    break;
    case BUTTON_RIGHT:
      playSimpleTone(NOTE_A7, 100);
      playNoTone();
      decrementState();
    break;
  }
  Serial << "count: " << counter << endl;
}

// probably not necessary to do anything here
void buttonReleaseCallback(uint8_t n) {

}

